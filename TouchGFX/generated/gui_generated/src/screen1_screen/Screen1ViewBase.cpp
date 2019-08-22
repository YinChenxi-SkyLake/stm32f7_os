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

    buttonDown.setXY(0, 106);
    buttonDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    buttonDown.setLabelText(TypedText(T_SINGLEUSEID3));
    buttonDown.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonDown.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonDown.setAction(buttonCallback);

    textArea1.setPosition(5, 57, 67, 49);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(222, 15, 15));
    textArea1.setLinespacing(0);
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%s", TypedText(T_SINGLEUSEID8).getText());
    textArea1.setWildcard(textArea1Buffer);
    textArea1.setTypedText(TypedText(T_SINGLEUSEID7));

    containerlDialog.setPosition(100, 46, 280, 180);
    containerlDialog.setVisible(false);

    imageDialogBg.setXY(0, 0);
    imageDialogBg.setBitmap(Bitmap(BITMAP_MODAL_BACKGROUND_ID));
    imageDialogBg.setAlpha(229);
    containerlDialog.add(imageDialogBg);

    dialogMsg.setPosition(37, 34, 213, 56);
    dialogMsg.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    dialogMsg.setLinespacing(0);
    dialogMsg.setTypedText(TypedText(T_DIALOGMSG));
    containerlDialog.add(dialogMsg);

    buttonCancel.setXY(18, 113);
    buttonCancel.setBitmaps(Bitmap(BITMAP_CANCEL_ID), Bitmap(BITMAP_CANCEL_PRESSED_ID));
    buttonCancel.setAction(buttonCallback);
    containerlDialog.add(buttonCancel);

    buttonOk.setXY(154, 112);
    buttonOk.setBitmaps(Bitmap(BITMAP_OK_ID), Bitmap(BITMAP_OK_PRESSED_ID));
    buttonOk.setAction(buttonCallback);
    containerlDialog.add(buttonOk);

    buttonShowDialog.setXY(0, 212);
    buttonShowDialog.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID));
    buttonShowDialog.setAction(buttonCallback);

    add(Image1);
    add(buttonUp);
    add(buttonDown);
    add(textArea1);
    add(containerlDialog);
    add(buttonShowDialog);
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
    else if (&src == &buttonCancel)
    {
        //InteractionButtonCancel
        //When buttonCancel clicked hide containerlDialog
        //Hide containerlDialog
        containerlDialog.setVisible(false);
        containerlDialog.invalidate();
    }
    else if (&src == &buttonOk)
    {
        //InteractionButtonOk
        //When buttonOk clicked hide containerlDialog
        //Hide containerlDialog
        containerlDialog.setVisible(false);
        containerlDialog.invalidate();
    }
    else if (&src == &buttonShowDialog)
    {
        //InteractionButtonShowDialog
        //When buttonShowDialog clicked show containerlDialog
        //Show containerlDialog
        containerlDialog.setVisible(true);
        containerlDialog.invalidate();
    }
}
