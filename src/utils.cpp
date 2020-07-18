#include "utils.h"

String getUniqueID()
{
  uint32_t mac = (ESP.getEfuseMac() >> 24) & 0xFFFFFF;
  mac = ((mac & 0xFF) << 16) + (mac & 0x00FF00) + ((mac & 0xFF0000) >> 16);
  return String(mac, HEX);
}

String build_hostname()
{
  String hostname = "TAP_" + getUniqueID();
  hostname.toUpperCase();
  return hostname;
}