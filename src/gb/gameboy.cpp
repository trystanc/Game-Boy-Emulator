
#include "gameboy.h"
GameBoy::GameBoy(std::ifstream &file)
    : m_cartridge(file),
      m_addressBus(m_cartridge),
      m_cpu(m_addressBus)
{
}
