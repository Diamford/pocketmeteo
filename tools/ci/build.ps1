param(
  [string]$Fqbn = "esp32:esp32:esp32c3",
  [string]$CoreVersion = "3.3.8"
)

$ErrorActionPreference = "Stop"

Write-Host "== pocketmeteo CI build =="
Write-Host "FQBN: $Fqbn"
Write-Host "ESP32 core: $CoreVersion"

if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
  throw "arduino-cli not found in PATH"
}

arduino-cli config init --overwrite | Out-Null
arduino-cli config set board_manager.additional_urls "https://espressif.github.io/arduino-esp32/package_esp32_index.json" | Out-Null

arduino-cli core update-index
arduino-cli core install "esp32:esp32@$CoreVersion"

arduino-cli lib update-index
$libs = Get-Content "pocketmeteo/libraries.txt" | Where-Object { $_ -and -not $_.StartsWith("#") }
foreach ($lib in $libs) {
  arduino-cli lib install --no-deps --yes "$lib"
}

arduino-cli compile -b "$Fqbn" "pocketmeteo/"

Write-Host "OK"

