#ifndef __ICONTROL_H__
#define __ICONTROL_H__

#include "IElement.h"
enum Event
{
    clicked, check, uncheck
};

// 控件基类

class IControl :public IElement
{
public:
    IControl(int width, int height);
    ~IControl();
    
    // 鼠标左键按下（在控件捕获范围内）
    virtual void LMBDown();
    // 鼠标左键弹起（在控件捕获范围内）
    virtual void LMBUp();

    // 获得焦点
    virtual void getFocus();
    // 失去焦点
    virtual void loseFocus();

    // 测试鼠标是否落在在元素的捕获范围内
    virtual bool ifMouseIn(int x, int y);
    // 鼠标移到控件范围外
    virtual void mouseMoveOut();
    // 鼠标移到控件上
    virtual void mouseMoveIn();
};

#endif

