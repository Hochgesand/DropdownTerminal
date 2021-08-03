#include "ApplicationManager.h"

#include <future>
#include <memory>
#include <WindowGrabber.h>
#include <algorithm>

#include "AppManagerObserver.h"

ApplicationManager::ApplicationManager()
{
	refreshRunningApps();
}

void ApplicationManager::refreshRunningApps()
{
    openApplications = WindowGrabber::getOpenWindowsApplications();
    for (auto& selected_application : selectedApplications)
    {
	    if (*selected_application->getTerminator() == false)
	    {
            deselectTerm(selected_application->getApplicationHook()->getApplicationInformation()->getAppName());
	    }
    }
}

// dd steht nicht f�r DOPPEL D sondern f�r DropDown
void ApplicationManager::select_application_for_dd(std::string app_name, unsigned int hotkey, unsigned int modHotkey)
{
    for (auto const element : *openApplications)
    {
        if (element.second == app_name)
        {        	
            selectedApplications.push_back(std::make_shared<ApplicationPositioning>(Application_Hook(element.first, element.second), hotkey, modHotkey));
            notify();
            break;
        }
    }
}

std::shared_ptr<std::map<HWND, std::string>> ApplicationManager::getOpenApps()
{
    return openApplications;
}

void ApplicationManager::eraseSelectedApplication(std::shared_ptr<ApplicationPositioning> element)
{
    selectedApplications.erase(
        std::remove(selectedApplications.begin(), selectedApplications.end(), element),
        selectedApplications.end()
    );
}

void ApplicationManager::deselectTerm(std::string appname)
{
	for (auto element : selectedApplications)
	{
		if(element == nullptr)
		{
            eraseSelectedApplication(element);
			continue;
		}
		if (element->getApplicationHook()->getApplicationInformation()->getAppName() == appname)
		{
            element->dropTerminal();
            element->unfocusApplication();
            element->terminate();
            eraseSelectedApplication(element);
            break;
		}
	}
    notify();
}

void ApplicationManager::deselectTerm()
{
    for (auto element : selectedApplications)
    {
	    if (element == nullptr)
	    {
		    continue;
	    }
        element->dropTerminal();
        element->unfocusApplication();
        element->terminate();
    }
    selectedApplications.clear();
    notify();
}

std::vector<std::string> ApplicationManager::getHookedApps()
{
    refreshRunningApps();
	std::vector<std::string> hookedApps;
	for (auto& element : selectedApplications)
	{
		hookedApps.push_back(element->getApplicationHook()->getApplicationInformation()->getAppName());
	}
    
	return hookedApps;
}

void ApplicationManager::notify()
{
	for (auto element : observers_)
	{
        element->Update();
	}
}

void ApplicationManager::attach(AppManagerObserver* amo)
{
	observers_.push_back(amo);
}

void ApplicationManager::detach(AppManagerObserver* amo)
{
	observers_.erase(std::remove(observers_.begin(), observers_.end(), amo), observers_.end());
}

