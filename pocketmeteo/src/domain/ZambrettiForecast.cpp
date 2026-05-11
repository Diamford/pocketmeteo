#include "ZambrettiForecast.h"

namespace pm::domain {

namespace {

ZambrettiResult chooseRising(float p) {
  if (p >= 1030.0f) {
    return {'A', "Устойчиво ясно"};
  }
  if (p >= 1022.0f) {
    return {'B', "Ясно, возможна небольшая облачность"};
  }
  if (p >= 1014.0f) {
    return {'C', "Переменная облачность, без осадков"};
  }
  return {'D', "Погода улучшается, но возможны кратковременные осадки"};
}

ZambrettiResult chooseSteady(float p, Season season) {
  if (p >= 1028.0f) {
    return {'E', "Стабильно ясно"};
  }
  if (p >= 1018.0f) {
    return {'F', "Облачно с прояснениями"};
  }
  if (p >= 1008.0f) {
    return {'G', season == Season::Summer ? "Вероятен кратковременный дождь"
                                          : "Вероятен снег или мокрый снег"};
  }
  return {'H', "Неустойчивая погода, вероятны осадки"};
}

ZambrettiResult chooseFalling(float p) {
  if (p >= 1022.0f) {
    return {'I', "Облачность увеличивается"};
  }
  if (p >= 1012.0f) {
    return {'J', "Вероятен дождь"};
  }
  if (p >= 1002.0f) {
    return {'K', "Дождь и усиление ветра"};
  }
  return {'L', "Ненастье, вероятны сильные осадки"};
}

} // namespace

ZambrettiResult ZambrettiForecast::forecast(float sea_level_pressure_hpa,
                                            float delta3h_hpa,
                                            Season season) {
  if (delta3h_hpa > 1.0f) {
    return chooseRising(sea_level_pressure_hpa);
  }
  if (delta3h_hpa < -1.0f) {
    return chooseFalling(sea_level_pressure_hpa);
  }
  return chooseSteady(sea_level_pressure_hpa, season);
}

} // namespace pm::domain
