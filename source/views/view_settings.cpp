#include "views/view_settings.hpp"

#include <hex/api/content_registry.hpp>

#include <nlohmann/json.hpp>

namespace hex {

    ViewSettings::ViewSettings() : View("hex.view.settings.name") {
        EventManager::subscribe<RequestOpenWindow>(this, [this](const std::string &name) {
            if (name == "Settings") {
                View::doLater([]{ ImGui::OpenPopup(View::toWindowName("hex.view.settings.name").c_str()); });
                this->getWindowOpenState() = true;
            }
        });
    }

    ViewSettings::~ViewSettings() {
        EventManager::unsubscribe<RequestOpenWindow>(this);
    }

    void ViewSettings::drawContent() {

        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));

        if (ImGui::BeginPopupModal(View::toWindowName("hex.view.settings.name").c_str(), &this->getWindowOpenState(), ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::BeginTabBar("settings")) {
                for (auto &[category, entries] : ContentRegistry::Settings::getEntries()) {
                    if (ImGui::BeginTabItem(LangEntry(category))) {
                        ImGui::TextUnformatted(LangEntry(category));
                        ImGui::Separator();

                        for (auto &[name, callback] : entries) {
                            if (callback(LangEntry(name), ContentRegistry::Settings::getSettingsData()[category][name]))
                                EventManager::post<EventSettingsChanged>();
                        }

                        ImGui::EndTabItem();
                    }
                }

                ImGui::EndTabBar();
            }
            ImGui::EndPopup();
        } else
            this->getWindowOpenState() = false;

    }

    void ViewSettings::drawMenu() {
        if (ImGui::BeginMenu("hex.menu.help"_lang)) {
            if (ImGui::MenuItem("hex.view.settings.name"_lang)) {
                View::doLater([]{ ImGui::OpenPopup(View::toWindowName("hex.view.settings.name").c_str()); });
                this->getWindowOpenState() = true;
            }

            ImGui::EndMenu();
        }
    }

}