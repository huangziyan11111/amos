#include "InsertWindow.hh"
#include "InsertWidget.hh"

#include <qstatusbar.h>
#include <qlabel.h>
#include <qslider.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>
#include <qmenubar.h>
#include <qpixmap.h>
#include <qpainter.h>

#include "UIElements.hh"


using namespace std;

InsertWindow::InsertWindow(DataStore * datastore,
                           QWidget * parent,
                           const char * name)
  : QMainWindow(parent, name)
{
  resize(600, 300);
  setCaption("Inserts");

  statusBar()->message("Ready", 2000);
  statusBar()->setSizeGripEnabled(false);

  // Toolbar
  QToolBar * options = new QToolBar(this, "options");
  options->setLabel("Options");

  new QLabel("Zoom", options, "zoomlbl");
  QSlider * zoom = new QSlider(1, 500, 10, 20, Qt::Horizontal, options, "zoom");


  // MenuBar
  m_typesmenu = new QPopupMenu(this);
  menuBar()->insertItem("&Show Types", m_typesmenu);

  const char * states = Insert::allstates;
  for (unsigned int i = 0; i < strlen(states); i++)
  {
    char state = states[i];
    QPixmap rect(10,10);

    QPainter p(&rect);
    p.fillRect(rect.rect(), UIElements::getInsertColor((Insert::MateState)state));
    p.end();

    QString name = Insert::getInsertTypeStr((Insert::MateState)state);

    m_types[state].first = m_typesmenu->insertItem(QIconSet(rect), name);
    m_types[state].second = true;
    m_typesmenu->setItemChecked(m_types[state].first, m_types[state].second);
  }

  // Main Widget
  InsertWidget * iw = new InsertWidget(datastore, m_types, this, "iw");
  setCentralWidget(iw);

  connect(m_typesmenu, SIGNAL(activated(int)),
          this,        SLOT(toggleItem(int)));

  connect(iw,          SIGNAL(setStatus(const QString &)),
          statusBar(), SLOT(message(const QString &)));

  connect(iw,   SIGNAL(setGindex(int)),
          this, SIGNAL(setGindex(int)));

  connect(this, SIGNAL(setTilingVisibleRange(int, int)),
          iw,   SLOT(setTilingVisibleRange(int, int)));

  connect(zoom, SIGNAL(valueChanged(int)),
          iw,   SLOT(setZoom(int)));

  connect(this, SIGNAL(refreshCanvas()),
          iw,   SLOT(refreshCanvas()));
}

void InsertWindow::contigChanged()
{
  emit refreshCanvas();
}

void InsertWindow::toggleItem(int id)
{
  typemap::iterator mi;

  for (mi = m_types.begin(); mi != m_types.end(); mi++)
  {
    if (mi->second.first == id)
    {
      mi->second.second = !m_typesmenu->isItemChecked(mi->second.first);
      m_typesmenu->setItemChecked(mi->second.first, mi->second.second);

      emit refreshCanvas();
      break;
    }
  }
}
