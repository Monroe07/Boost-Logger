#include "Logger.h"

int main(void){
  Logger::initFromConfig("logconfig.ini");
  LOG_INFO("This is a Log Entry at Info Severity");
  return 0;
}
