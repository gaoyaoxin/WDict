#ifndef _WITHTITLECONSOLE_H_
#define _WITHTITLECONSOLE_H_

#include <iostream>
#include <string>
#include <memory>
#include "ConsoleComponent.h"
#include "Console.h"
#include <cstdlib>

class WithTitleConsole
    : public ConsoleComponent
{
private:
    std::string title;
    std::shared_ptr<ConsoleComponent> comp;
public:
    WithTitleConsole(Console &Root, const std::string &title)
        : ConsoleComponent(Root), title(title), comp(nullptr)
    {
        
    }
    
    WithTitleConsole(const std::string &title, std::shared_ptr<ConsoleComponent> comp)
        : ConsoleComponent(comp->Root), title(title), comp(comp)
    {
        
    }
    bool Show() override;
};

#endif // _WITHTITLECONSOLE_H_