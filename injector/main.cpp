#include "ManualMap.h"
#include "LoadLibrary.h"

enum InjectionType : int
{
    ManualMap,
    loadLibrary
};

InjectionType InjectionMethod;
string proccessname;
string dllname;


int main() 
{
    printf("Simple ManualMap/LoadLibrary injector by PolandCheater\n\nPlease choose method of injection - press Enter for ManualMap or 1 for LoadLibrary\n");
    if (cin.get() == '\n')
        InjectionMethod = ManualMap;
    else { InjectionMethod = loadLibrary;  cin.get(); }
    
    printf("Please input proccess you want inject to (press Enter for csgo.exe)\n");

    getline(cin, proccessname);
    if (proccessname.empty()) proccessname = "csgo.exe";

    printf("Please input dll you want to inject (press Enter for memehook.dll)\n");

    getline(cin, dllname);
    if (dllname.empty()) dllname = "memehook.dll";

    switch (InjectionMethod)
    {
    case ManualMap:
        manual_map->manualmapmain(proccessname.c_str(), dllname.c_str());
        break;
    case loadLibrary:
        load_library->loadlibrarymain(proccessname.c_str(), dllname.c_str());
        break;
    }
}

