#include "PVZHelper.Cli.h"
#include "PVZ_1_0.h"
#include "PVZ_1_2_CN_ENDLESS.h"
#include "PVZ_1_2_CN_REPAIR.h"
#include "PVZ_1_2_EN_1073.h"

using std::make_unique;
using std::unique_ptr;
using namespace System::Runtime::InteropServices;

namespace PVZHelper::Cli
{
    InitErr PVZController::InitGame()
    {
        unique_ptr<Process> process = make_unique<Process>();
        switch (process->OpenByName(L"popcapgame1.exe"))
        {
        case NOT_FOUND:
            switch (process->OpenByName(L"PlantsVsZombies.exe"))
            {
            case NOT_FOUND:
                return InitErr::ExeNotFound;
            case OPEN_FAILED:
                return InitErr::OpenFailed;
            case SUCCESS:
                break;
            }
            break;
        case OPEN_FAILED:
            return InitErr::OpenFailed;
        case SUCCESS:
            break;
        }
        if (process->IsValid())
        {
            if (process->ReadMemory(4, 0x42748e) == 0xff563de8)
            {
                pvz = make_cli<PVZ_1_0>(std::move(process));
            }
            else if (process->ReadMemory(4, 0x429a83) == 0xffff7489)
            {
                pvz = make_cli<PVZ_1_2_EN_1073>(std::move(process));
            }
            else if (process->ReadMemory(4, 0x428902) == 0x03c109e8)
            {
                pvz = make_cli<PVZ_1_2_CN_REPAIR>(std::move(process));
            }
            else if (process->ReadMemory(4, 0x434d57) == 0x5b5d5e5f)
            {
                pvz = make_cli<PVZ_1_2_CN_ENDLESS>(std::move(process));
            }
            else
            {
                pvz = nullptr;
                return InitErr::NotSupport;
            }
            pvz->Patch();
            pvz->InitExtraCode();
        }
        return InitErr::Success;
    }

    void PVZController::NaturalSeed(array<int>^ zombies)
    {
        pin_ptr<int> pz = &zombies[0];
        pvz->NaturalSeed((int*)pz, zombies->Length);
    }

    void PVZController::LimitSeed(array<int>^ zombies, bool thief, bool red)
    {
        pin_ptr<int> pz = &zombies[0];
        pvz->LimitSeed((int*)pz, zombies->Length, thief, red);
    }

    array<bool>^ PVZController::SeeLeftZombies()
    {
        bool zombies[33];
        pvz->SeeLeftZombies(zombies);
        array<bool>^ result = gcnew array<bool>(33);
        pin_ptr<bool> pr = &result[0];
        std::copy(std::begin(zombies), std::end(zombies), (bool*)pr);
        return result;
    }
} // namespace PVZHelper::Cli
