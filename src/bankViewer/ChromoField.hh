#ifndef CHROMOFIELD_HH_
#define CHROMOFIELD_HH_

#include <qwidget.h>
#include <string>
#include "RenderSeq.hh"

#ifdef __cplusplus
extern "C"
{
#endif

#include <Read.h>

#ifdef __cplusplus
}
#endif

class ChromoField: public QWidget
{
  Q_OBJECT
public:
    ChromoField(RenderSeq_t * read, string & db, QWidget *parent=0, const char *name=0);

protected:
    void paintEvent( QPaintEvent * );

private:
    Read * m_rawread;
    RenderSeq_t * m_render;

};



#endif
