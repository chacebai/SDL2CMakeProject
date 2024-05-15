#include "StaticText.h"



StaticText::StaticText(int width, int height, TCHAR *text)
    :IControl(width, height), m_text(text)
{
}


StaticText::~StaticText()
{
}


// ����ˮƽ���뷽ʽ
void StaticText::setTextHorizontalAlignment(Alignment align)
{
    if (align == left || align == center || align == right)
    {
        m_horizontalAlignment = align;
    }
}



// ���ô�ֱ�����ϵĶ��뷽ʽ
void StaticText::setTextVerticalAlignment(Alignment align)
{
    if (align == top || align == center || align == bottom)
    {
        m_verticalAlignment = align;
    }
}


void StaticText::setText(TCHAR *text)
{
    m_text = text;
}


// ����
void StaticText::draw(Graphics &g)
{
}