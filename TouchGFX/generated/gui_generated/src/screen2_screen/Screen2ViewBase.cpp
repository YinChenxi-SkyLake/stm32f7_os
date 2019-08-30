/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

Screen2ViewBase::Screen2ViewBase() :
    buttonCallback(this, &Screen2ViewBase::buttonCallbackHandler),
    flexButtonCallback(this, &Screen2ViewBase::flexButtonCallbackHandler)
{
    image.setXY(0, 0);
    image.setBitmap(Bitmap(BITMAP_BK2_ID));

    buttonGotoScreen1.setXY(0, 106);
    buttonGotoScreen1.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID), Bitmap(BITMAP_BLUE_ICONS_BACK_ARROW_32_ID));
    buttonGotoScreen1.setIconXY(22, 15);
    buttonGotoScreen1.setAction(buttonCallback);
    buttonGotoScreen1.setAlpha(132);

    flexButtonSpeedAdd.setDelay(12);
    flexButtonSpeedAdd.setInterval(60);
    flexButtonSpeedAdd.setBitmaps(Bitmap(BITMAP_SPEEDCTRL_ID), Bitmap(BITMAP_SPEEDCTRLPRESSED_ID));
    flexButtonSpeedAdd.setBitmapXY(0, 0);
    flexButtonSpeedAdd.setText(TypedText(T_SINGLEUSEID9));
    flexButtonSpeedAdd.setTextPosition(0, 15, 80, 80);
    flexButtonSpeedAdd.setTextColors(touchgfx::Color::getColorFrom24BitRGB(10, 10, 10), touchgfx::Color::getColorFrom24BitRGB(10, 10, 10));
    flexButtonSpeedAdd.setPosition(400, 192, 80, 80);
    flexButtonSpeedAdd.setAction(flexButtonCallback);

    flexButtonSpeedMinus.setDelay(12);
    flexButtonSpeedMinus.setInterval(60);
    flexButtonSpeedMinus.setBitmaps(Bitmap(BITMAP_SPEEDCTRL_ID), Bitmap(BITMAP_SPEEDCTRLPRESSED_ID));
    flexButtonSpeedMinus.setBitmapXY(0, 0);
    flexButtonSpeedMinus.setText(TypedText(T_SINGLEUSEID10));
    flexButtonSpeedMinus.setTextPosition(0, 0, 80, 80);
    flexButtonSpeedMinus.setTextColors(touchgfx::Color::getColorFrom24BitRGB(10, 10, 10), touchgfx::Color::getColorFrom24BitRGB(10, 10, 10));
    flexButtonSpeedMinus.setPosition(314, 192, 80, 80);
    flexButtonSpeedMinus.setAction(flexButtonCallback);

    add(image);
    add(buttonGotoScreen1);
    add(flexButtonSpeedAdd);
    add(flexButtonSpeedMinus);
}

void Screen2ViewBase::setupScreen()
{

}

void Screen2ViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonGotoScreen1)
    {
        //InteractionButtonGotoScreen1
        //When buttonGotoScreen1 clicked change screen to Screen1
        //Go to Screen1 with screen transition towards West
        application().gotoScreen1ScreenSlideTransitionWest();
    }
}

void Screen2ViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &flexButtonSpeedAdd)
    {
        //InteractionSpeedAdd
        //When flexButtonSpeedAdd clicked call virtual function
        //Call addFanSpeed
        addFanSpeed();
    }
    else if (&src == &flexButtonSpeedMinus)
    {
        //InteractionSpeedMinus
        //When flexButtonSpeedMinus clicked call virtual function
        //Call minusFanSpeed
        minusFanSpeed();
    }
}
