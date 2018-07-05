#ifndef DPERSISTENTMAINWINDOW_H
#define DPERSISTENTMAINWINDOW_H

/*****************************************************************************
 ******************************  I N C L U D E  ******************************
 ****************************************************************************/

#include <QMainWindow>
#include <QString>
#include <QCloseEvent>

/*****************************************************************************
 *
 ***  class DPersistentMainWindow
 *
 * Window class the provides for handing persistent parameters.  Derived
 * classes have to override LoadParameters() and SaveParameters() to suit
 * their particular needs.  This class provides automatic calls on startup and
 * shutdown to handle loading from the file cache warning that defaults will
 * be used and prompting the user if saving is desired.
 *
 *****************************************************************************/

class DPersistentMainWindow : public QMainWindow
   {
   Q_OBJECT

   using Base = QMainWindow;

   public:
      DPersistentMainWindow(QWidget* pParent = nullptr);

      virtual ~DPersistentMainWindow();

      virtual void Initialize();

   protected:
      QString m_strAppName;
      bool m_bPersistenceEnabled;

      virtual void SetupCentralWidget();
      virtual void closeEvent(QCloseEvent* pEvent);
      virtual QString GetParameterPath(const QString& strExt) const;

   protected slots:
      virtual bool LoadParameters();
      virtual bool SaveParameters();

   private:
      DPersistentMainWindow(const DPersistentMainWindow& src);
      DPersistentMainWindow& operator=(const DPersistentMainWindow& rhs);

   }; // end of class DPersistentMainWindow

#endif // DPERSISTENTMAINWINDOW_H
