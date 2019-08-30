/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

Screen1ViewBase::Screen1ViewBase() :
    buttonCallback(this, &Screen1ViewBase::buttonCallbackHandler)
{
    Image1.setXY(0, 0);
    Image1.setBitmap(Bitmap(BITMAP_MN_ID));

    buttonUp.setXY(0, 0);
    buttonUp.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    buttonUp.setLabelText(TypedText(T_SINGLEUSEID2));
    buttonUp.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonUp.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonUp.setAction(buttonCallback);

    buttonDown.setXY(60, 0);
    buttonDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    buttonDown.setLabelText(TypedText(T_SINGLEUSEID3));
    buttonDown.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonDown.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonDown.setAction(buttonCallback);

    textArea1.setPosition(130, 6, 67, 49);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(222, 15, 15));
    textArea1.setLinespacing(0);
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%s", TypedText(T_SINGLEUSEID8).getText());
    textArea1.setWildcard(textArea1Buffer);
    textArea1.setTypedText(TypedText(T_SINGLEUSEID7));

    buttonGotoScreen2.setXY(420, 118);
    buttonGotoScreen2.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID), Bitmap(BITMAP_BLUE_ICONS_NEXT_ARROW_32_ID));
    buttonGotoScreen2.setIconXY(22, 15);
    buttonGotoScreen2.setAction(buttonCallback);
    buttonGotoScreen2.setAlpha(135);

    add(Image1);
    add(buttonUp);
    add(buttonDown);
    add(textArea1);
    add(buttonGotoScreen2);
}

void Screen1ViewBase::setupScreen()
{

}

void Screen1ViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonUp)
    {
        //InteractionButtonUp
        //When buttonUp clicked call virtual function
        //Call buttonUpClicked
        buttonUpClicked();
    }
    else if (&src == &buttonDown)
    {
        //InteractionButtonDown
        //When buttonDown clicked call virtual function
        //Call buttonDownClicked
        buttonDownClicked();
    }
    else if (&src == &buttonGotoScreen2)
    {
        //InteractionButtonGotoScreen2
        //When buttonGotoScreen2 clicked change screen to Screen2
        //Go to Screen2 with screen transition towards East
        application().gotoScreen2ScreenCoverTransitionEast();
    }
}
