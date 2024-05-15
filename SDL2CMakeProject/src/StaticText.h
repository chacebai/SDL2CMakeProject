#ifndef __ISRARICTEXT_H__
#define __ISRARICTEXT_H__

#include "IControl.h"
#include <string>
using std::string;

// ��̬�ı��ؼ�

class StaticText :
    public IControl
{
public:
    // ���뷽ʽö������
    enum Alignment{left, right, top, bottom, center};

    StaticText(int width, int height, TCHAR *text);
    ~StaticText();

    void setText(TCHAR *text);
    // ����ˮƽ���뷽ʽ
    void setTextHorizontalAlignment(Alignment align);
    // ���ô�ֱ�����ϵĶ��뷽ʽ
    void setTextVerticalAlignment(Alignment align);
    // ����
    virtual void draw(Graphics &g) override;

protected:
    const char *m_text = "static text"; // �ı�
    Alignment   m_horizontalAlignment = center; // ˮƽ���뷽ʽ
    Alignment   m_verticalAlignment = center;   // ��ֱ���뷽ʽ
};

#endif

