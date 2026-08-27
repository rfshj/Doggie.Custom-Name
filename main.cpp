#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;
using namespace cocos2d;

namespace local_name {

static bool enabled() {
    return Mod::get()->getSettingValue<bool>("enabled");
}

static std::string customName() {
    return Mod::get()->getSettingValue<std::string>("custom-name");
}

class NamePopup final : public Popup {
protected:
    geode::TextInput* m_input = nullptr;
    CCMenuItemToggler* m_toggle = nullptr;

    bool init() {
        if (!Popup::init(360.f, 210.f, "GJ_square01.png", {0, 0, 80, 80}))
            return false;

        this->setTitle("Custom Name", "goldFont.fnt", 1.0f, 20.f);

        // Custom icon based on the new mod identity.
        if (auto icon = CCSprite::create("CN_nameIcon_001.png")) {
            icon->setScale(0.55f);
            icon->setPosition({180.f, 157.f});
            this->m_mainLayer->addChild(icon);
        }

        auto title = CCLabelBMFont::create(
            "Change your local display name",
            "bigFont.fnt"
        );
        title->setScale(0.45f);
        title->setPosition({180.f, 128.f});
        this->m_mainLayer->addChild(title);

        m_input = geode::TextInput::create(300.f, "Custom Name", "bigFont.fnt");
        m_input->setMaxCharCount(30);
        m_input->setString(customName(), false);
        m_input->setTextAlign(geode::TextInputAlign::Left);
        m_input->setCallback([](std::string const& value) {
            Mod::get()->setSettingValue("custom-name", value);
        });
        m_input->getInputNode()->setLabelPlaceholderColor({105, 105, 105});
        m_input->setPosition({180.f, 91.f});
        this->m_mainLayer->addChild(m_input);

        auto offLabel = CCLabelBMFont::create("Enabled: OFF", "goldFont.fnt");
        offLabel->setScale(0.55f);
        auto onLabel = CCLabelBMFont::create("Enabled: ON", "goldFont.fnt");
        onLabel->setScale(0.55f);

        m_toggle = CCMenuItemToggler::create(
            offLabel,
            onLabel,
            this,
            menu_selector(NamePopup::onToggle)
        );
        m_toggle->toggle(enabled());
        m_toggle->setPosition({180.f, 46.f});

        auto menu = CCMenu::create();
        menu->setPosition({0.f, 0.f});
        menu->addChild(m_toggle);
        this->m_mainLayer->addChild(menu);

        auto note = CCLabelBMFont::create(
            "Visual only - your real account name is never changed",
            "goldFont.fnt"
        );
        note->setScale(0.36f);
        note->setOpacity(170);
        note->setPosition({180.f, 17.f});
        this->m_mainLayer->addChild(note);

        return true;
    }

    void onToggle(CCObject*) {
        auto newValue = !enabled();
        Mod::get()->setSettingValue("enabled", newValue);
        m_toggle->toggle(newValue);
    }

public:
    static NamePopup* create() {
        auto ret = new NamePopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

static void openMenu() {
    if (auto popup = NamePopup::create())
        popup->show();
}

} // namespace local_name

$on_game(Loaded) {
    listenForKeybindSettingPresses(
        "open-menu",
        [](Keybind const&, bool down, bool repeat, double) {
            if (down && !repeat)
                local_name::openMenu();
        }
    );
}
