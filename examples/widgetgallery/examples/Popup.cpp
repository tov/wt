#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WMenuItem.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

SAMPLE_BEGIN(Popup)

auto container = Wt::cpp14::make_unique<Wt::WContainerWidget>();

auto popupPtr = Wt::cpp14::make_unique<Wt::WPopupMenu>();
auto popup = popupPtr.get();

auto statusPtr = Wt::cpp14::make_unique<Wt::WText>();
auto status = statusPtr.get();
status->setMargin(10, Wt::Side::Left | Wt::Side::Right);

auto outPtr = Wt::cpp14::make_unique<Wt::WText>();
auto out = outPtr.get();

// Create some menu items for the popup menu
popup->addItem("Connect")->triggered().connect(std::bind([=] () {
    out->setText("<p>Connecting...</p>");
}));

popup->addItem("Disconnect")->triggered().connect(std::bind([=] () {
    out->setText("<p>You are disconnected now.</p>");
}));

popup->addSeparator();

popup->addItem("icons/house.png", "I'm home")->triggered().connect(std::bind([=] () {
    out->setText("");
}));

Wt::WMenuItem *item = popup->addItem("Don't disturb");
item->setCheckable(true);

item->triggered().connect(std::bind([=] () {
    out->setText(Wt::WString::fromUTF8("<p>{1} item is {2}.</p>")
		 .arg(item->text())
		 .arg(item->isChecked() ? "checked" : "unchecked"));
}));

popup->addSeparator();

// Create a submenu for the popup menu.
auto subMenuPtr = Wt::cpp14::make_unique<Wt::WPopupMenu>();
auto subMenu = subMenuPtr.get();

subMenu->addItem("Contents")->triggered().connect(std::bind([=] () {
    out->setText("<p>This could be a link to /contents.html.</p>");
}));

subMenu->addItem("Index")->triggered().connect(std::bind([=] () {
    out->setText("<p>This could be a link to /index.html.</p>");
}));

subMenu->addSeparator();
subMenu->addItem("About")->triggered().connect(std::bind([=] () {
    auto messageBoxPtr = Wt::cpp14::make_unique<Wt::WMessageBox>
        ("About", "<p>This is a program to make connections.</p>",
         Wt::Icon::Information, Wt::StandardButton::Ok);
    auto messageBox = messageBoxPtr.get();
    messageBox->show();
    messageBox->buttonClicked().connect(std::bind([=] () {
      subMenu->removeChild(messageBox);
    }));
    subMenu->addChild(std::move(messageBoxPtr));
}));

// Assign the submenu to the parent popup menu.
popup->addMenu("Help", std::move(subMenuPtr));

Wt::WPushButton *button = container->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>());
button->setMenu(std::move(popupPtr));

// React to an item selection
popup->itemSelected().connect(std::bind([=] (Wt::WMenuItem *item) {
    status->setText
        (Wt::utf8("Selected menu item: {1}.")
	 .arg(item->text()));
}, std::placeholders::_1));

container->addWidget(std::move(statusPtr));
container->addWidget(std::move(outPtr));

SAMPLE_END(return std::move(container))
