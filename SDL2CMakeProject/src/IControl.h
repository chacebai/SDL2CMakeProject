#ifndef __ICONTROL_H__
#define __ICONTROL_H__

#include "IElement.h"
enum Event
{
    clicked, check, uncheck
};

// �ؼ�����

class IControl :public IElement
{
public:
    IControl(int width, int height);
    ~IControl();
    
    // ���������£��ڿؼ�����Χ�ڣ�
    virtual void LMBDown();
    // �����������ڿؼ�����Χ�ڣ�
    virtual void LMBUp();

    // ��ý���
    virtual void getFocus();
    // ʧȥ����
    virtual void loseFocus();

    // ��������Ƿ�������Ԫ�صĲ���Χ��
    virtual bool ifMouseIn(int x, int y);
    // ����Ƶ��ؼ���Χ��
    virtual void mouseMoveOut();
    // ����Ƶ��ؼ���
    virtual void mouseMoveIn();
};

#endif
