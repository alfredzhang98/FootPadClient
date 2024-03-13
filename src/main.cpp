#include <main.h>
#include <tool_logger.h>
#include <tool_timer.h>

bool debug_print = false;
bool log_print = false;

union {
    float f;
    byte floatBytes[4];
} floatContainer{};

std::vector<std::vector<uint8_t>> measurePattern;
tool_timer timer1;
tool_timer timer2;
bool pattern_flag = false;

unsigned long t;

void setup()
{
  Serial.begin(115200);
  Serial.flush();
  logger.enableLogging(log_print);
 
  /* Init MUX and AD5940 */
  setupMux();
  AD5940_BIA_Setup();
  print_setting_info();
}

void loop()
{

  while (Serial.available() == 0)
    ;
  uint8_t Usr_cmd = Serial.read();

  // 判断Usr_cmd是否为 SET_PATTERN_BEGIN = 126, SET_PATTERN_END = 127, GET_MEASUREMENT = 128
  // Serial Test: 7E 0B 0A 0B 0A 0B 09 0B 09 0B 08 0B 08 0B 07 0B 07 0B 06 0B 06 0B 05 0B 05 0B 04 0B 04 0B 03 0B 03 0B 02 0B 02 0B 01 0B 01 0C 0A 0C 0A 0C 09 0C 09 0C 08 0C 08 0C 07 0C 07 0C 06 0C 06 0C 05 0C 05 0C 04 0C 04 0C 03 0C 03 0C 02 0C 02 0C 01 0C 01 0D 0A 0D 0A 0D 09 0D 09 0D 08 0D 08 0D 07 0D 07 0D 06 0D 06 0D 05 0D 05 0D 04 0D 04 0D 03 0D 03 0D 02 0D 02 0D 01 0D 01 0E 0A 0E 0A 0E 09 0E 09 0E 08 0E 08 0E 07 0E 07 0E 06 0E 06 0E 05 0E 05 0E 04 0E 04 0E 03 0E 03 0E 02 0E 02 0E 01 0E 01 0F 0A 0F 0A 0F 09 0F 09 0F 08 0F 08 0F 07 0F 07 0F 06 0F 06 0F 05 0F 05 0F 04 0F 04 0F 03 0F 03 0F 02 0F 02 0F 01 0F 01 10 0A 10 0A 10 09 10 09 10 08 10 08 10 07 10 07 10 06 10 06 10 05 10 05 10 04 10 04 10 03 10 03 10 02 10 02 10 01 10 01 11 0A 11 0A 11 09 11 09 11 08 11 08 11 07 11 07 11 06 11 06 11 05 11 05 11 04 11 04 11 03 11 03 11 02 11 02 11 01 11 01 12 0A 12 0A 12 09 12 09 12 08 12 08 12 07 12 07 12 06 12 06 12 05 12 05 12 04 12 04 12 03 12 03 12 02 12 02 12 01 12 01 13 0A 13 0A 13 09 13 09 13 08 13 08 13 07 13 07 13 06 13 06 13 05 13 05 13 04 13 04 13 03 13 03 13 02 13 02 13 01 13 01 14 0A 14 0A 14 09 14 09 14 08 14 08 14 07 14 07 14 06 14 06 14 05 14 05 14 04 14 04 14 03 14 03 14 02 14 02 14 01 14 01 7F
  if (Usr_cmd == SET_PATTERN_BEGIN)
  {
    uint8_t dataBuffer[4];
    timer1.start();
    measurePattern.clear();
    do
    {
      while (Serial.available() == 0)
        ;
      dataBuffer[0] = Serial.read();
      if (dataBuffer[0] == SET_PATTERN_END)
        break;
      while (Serial.available() < 3)
        ;
      Serial.readBytes(dataBuffer + 1, 3);
      measurePattern.emplace_back(std::vector<uint8_t>{dataBuffer[0], dataBuffer[1], dataBuffer[2], dataBuffer[3]});
    } while (true);
    timer1.stop("SET_PATTERN_BEGIN");

    if(debug_print){
      logger.enableIdentifier(false);
      for (const auto &outer : measurePattern)
      {
        for (const auto &inner : outer)
        {

          logger.logf("%d ", static_cast<int>(inner));
        }
      }
      logger.logln("\nFinsih print Data");
      logger.enableIdentifier(true);
    }
    pattern_flag = true;
  }

  if (Usr_cmd == GET_MEASUREMENT)
  {
    if (!pattern_flag)
    {
      logger.logln("GET_MEASUREMENT without the pattern setting");
      return;
    }
    logger.logln("GET_MEASUREMENT prcessing...");

    AppBIACtrl(BIACTRL_START, nullptr);
          timer2.start();
      for (auto &i : measurePattern)
      {
        /* 更新Mux配置 */
        handleMux(i.data());
        /* 更新AD5940的测量结果 */
        AD5940_BIA_UpdateReading();
        /* 转换测量结果为byte数组并发送 */
        floatContainer.f = GetBIAResult();
        if(!log_print && !debug_print) Serial.write(floatContainer.floatBytes, 4);
        // logger.loglnf("AD5940MeasureResult: %f", floatContainer.f);
      }
      timer2.stop("HandleMux time");
          AppBIACtrl(BIACTRL_STOPNOW, nullptr);
  }
  delay(10);
}
