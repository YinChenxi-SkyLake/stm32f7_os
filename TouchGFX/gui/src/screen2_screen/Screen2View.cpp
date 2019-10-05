#include <gui/screen2_screen/Screen2View.hpp>
#include <BitmapDatabase.hpp>
Screen2View::Screen2View():    
    zAngle2D(0.0f),
    deltaZangle2D(0.0f)
{
    
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
    // Extra space for the image to rotate in
    int borderWidth = 40;
    int borderHeight = 40;
    
    // Setup the texture mapper image that does 2D rotation
    textureMapperImage2D.setBitmap(Bitmap(BITMAP_IMAGE00_ID));
    
    int imageWidth = textureMapperImage2D.getBitmap().getWidth();//��ͼƬ�Ŀ��
    int imageHeight = textureMapperImage2D.getBitmap().getHeight();//��ȡͼƬ�ĸ߶�
    
    textureMapperImage2D.setXY(100, 0);
    textureMapperImage2D.setWidth(imageWidth + borderWidth * 2);
    textureMapperImage2D.setHeight(imageHeight + borderHeight * 2);
    textureMapperImage2D.setBitmapPosition(borderWidth, borderHeight);
    textureMapperImage2D.setOrigo(textureMapperImage2D.getBitmapPositionX() + 97, textureMapperImage2D.getBitmapPositionY() + 108);

    add(textureMapperImage2D);
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleTickEvent()
{
    int x, y;
    
    zAngle2D += deltaZangle2D;
    
    // Update the images with the new angles
    // The image is automatically invalidated (the optimal minimal area).
    // If any of the set..() methods (e.g. setOrigo, setCamera, setRenderingAlgorithm...) are used
    // remember to manually invalidate the image afterwards (textureMapperImage2D.invalidate()).
    textureMapperImage2D.updateAngles(0.0F, 0.0F, zAngle2D);
    
    // sets the new angle to rotate circleRotate //
    circleRotate.invalidate();
    circleRotate.setArc(circleRotate.getArcStart() + 1, circleRotate.getArcEnd() + 1);
    circleRotate.invalidate();
    
        // Updates the coordinates to rotate lineRotate //
    lineRotate.invalidate();
    lineRotate.getStart(x, y);
    if (moveX)
    {
        if (x <= 10){
            lineRotateChangeFactor = -1;
            moveX = false;
        } else if (x >= 70){
            lineRotateChangeFactor = 1;
            moveX = false;
        }
    }
    else
    {
        if (y <= 10){
            lineRotateChangeFactor = 1;
            moveX = true;
        } else if (y >= 70) {
            lineRotateChangeFactor = -1;
            moveX = true;
        }
    }

    if (moveX)
    {
        lineRotate.setStart(x + lineRotateChangeFactor, y);
        lineRotate.getEnd(x, y);
        lineRotate.setEnd(x + (-lineRotateChangeFactor), y);
    }
    else
    {
        lineRotate.setStart(x, y + lineRotateChangeFactor);
        lineRotate.getEnd(x, y);
        lineRotate.setEnd(x, y + (-lineRotateChangeFactor));
    }

    lineRotate.invalidate();
}

void Screen2View::addFanSpeed()
{
    deltaZangle2D += 0.1f;
    if(deltaZangle2D > 1)
        deltaZangle2D = 1;
}

void Screen2View::minusFanSpeed()
{
    deltaZangle2D -= 0.1f;
    if(deltaZangle2D < 0){
        deltaZangle2D = 0;
    }
}

void Screen2View::setFanSpeed(int value)
{
    deltaZangle2D = value/10.0f;
}
