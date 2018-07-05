#pragma once

#ifndef DLABELEDWIDGET_H
#define DLABELEDWIDGET_H

/*****************************************************************************
 ******************************  I N C L U D E  ******************************
 ****************************************************************************/

#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>

#include <type_traits>
#include <assert.h>

/*****************************************************************************
 *
 ***  template <typename QWidget> class DLabeledWidget
 *
 * This class wraps a QWidget together with a QLabel to name it.  This pattern
 * occurs frequently and becomes tedious to keep reimplementing.  The original
 * use case was for Widgets from the QWT library.
 *
 * The constructors allow a case where the contained Widget is created using its
 * default constuctor (basically setting the Parent to nullptr) and then the
 * internal Widget instance is accessed using GetWidget() to set its properties.
 * Other variations allow creating the Widget externally using the constructor of
 * your choice and passing a pointer to the Widget through the constructor.
 *
 *****************************************************************************/

// TODO Restrict instantiation to only cases where TWIDGET is derived from base QWidget
template <typename TWIDGET>
class DLabeledWidget : public QFrame
   {
      static_assert(std::is_base_of_v<QWidget, TWIDGET>, "DLabeledWidget - TWIDGET must be derived from QWidget");
//      Q_OBJECT

   public:
      enum class ELabelPosition {eTop, eBottom};

      DLabeledWidget(QWidget* pParent = nullptr) : QFrame(pParent), m_pLabel{nullptr},
            m_eLabelPos{ELabelPosition::eBottom}
         {
         m_pWidget = new TWIDGET(this);

         InitLabel();

         InitLayout(m_eLabelPos);

         return;
         }

      DLabeledWidget(ELabelPosition eLabelPosition, QWidget* pParent = nullptr)
            : QFrame(pParent), m_pLabel{nullptr}, m_eLabelPos{eLabelPosition}
         {
         m_pWidget = new TWIDGET(this);

         InitLabel();

         InitLayout(m_eLabelPos);

         return;
         }

      DLabeledWidget(TWIDGET* pWidget, ELabelPosition eLabelPosition, QWidget* pParent = nullptr)
            : QFrame(pParent), m_pLabel{nullptr}, m_eLabelPos{eLabelPosition}
         {
         pWidget->setParent(this);
         m_pWidget = pWidget;

         InitLabel();

         InitLayout(m_eLabelPos);

         return;
         }

      DLabeledWidget(TWIDGET* pWidget, const QString& strLabel, ELabelPosition eLabelPosition, QWidget* pParent = nullptr)
            : QFrame(pParent), m_pLabel{nullptr},  m_eLabelPos{eLabelPosition}
         {
         pWidget->setParent(this);
         m_pWidget = pWidget;

         InitLabel(strLabel);

         InitLayout(m_eLabelPos);

         return;
         }

      DLabeledWidget(const QString& strLabel, ELabelPosition eLabelPosition, QWidget* pParent = nullptr)
            : QFrame(pParent), m_pLabel{nullptr}, m_eLabelPos{eLabelPosition}
         {
         m_pWidget = new TWIDGET(this);

         InitLabel(strLabel);

         InitLayout(m_eLabelPos);

         return;
         }

      DLabeledWidget(const DLabeledWidget& src) = delete;
      DLabeledWidget(const DLabeledWidget&& src) = delete;

      ~DLabeledWidget() = default;

      DLabeledWidget& operator=(const DLabeledWidget& rhs) = delete;
      DLabeledWidget& operator=(const DLabeledWidget&& rhs) = delete;

      TWIDGET* GetWidget()
         {
         return (m_pWidget);
         }

      const TWIDGET* GetWidget() const
         {
         return (m_pWidget);
         }

      void SetLabelText(QString strText)
         {
         m_pLabel->setText(strText);
         return;
         }

      QLabel* GetLabel()
         {
         return (m_pLabel);
         }

      const QLabel* GetLabel() const
         {
         return (m_pLabel);
         }

      QVBoxLayout* GetLayout()
         {
         return (m_pLayout);
         }

      const QVBoxLayout* GetLayout() const
         {
         return (m_pLayout);
         }

      ELabelPosition GetLabelPosition() const
         {
         return(m_eLabelPos);
         }
#if 0
      void SetLabelPosition(ELabelPosition eLabelPosition)
         {
         m_eLabelPosition = eLabelPosition;
         return;
         }
#endif
      virtual QSize sizeHint() const override
         {
         auto LabelHint = m_pLabel->sizeHint();
         auto WidgetHint = m_pWidget->sizeHint();

         auto Width = std::max(LabelHint.width(), WidgetHint.width());
         auto Height = LabelHint.height() + WidgetHint.height();

         return (QSize(Width, Height));
         }

   protected:
      TWIDGET* m_pWidget;
      QLabel* m_pLabel;
      QVBoxLayout* m_pLayout;
      ELabelPosition m_eLabelPos;

      void SetLabelSizePolicy()
         {
         if (m_pLabel != nullptr)
            {
            m_pLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            return;
            } // end if
         }

      void InitLabel(QString strLabel = QString())
         {
         m_pLabel = new QLabel(strLabel, this);
         m_pLabel->setAlignment(Qt::AlignCenter);
         SetLabelSizePolicy();

         return;
         }

      void InitLayout(ELabelPosition eLabelPosition)
         {
         m_pLayout = new QVBoxLayout(this);
         m_pLayout->setMargin(0);
         m_pLayout->setSpacing(0);

         if (m_eLabelPos == ELabelPosition::eTop)
            {
            m_pLayout->addWidget(m_pLabel);
            m_pLayout->addWidget(m_pWidget);
            } // end if
         else
            {
            m_pLayout->addWidget(m_pWidget);
            m_pLayout->addWidget(m_pLabel);
            } // end else

         return;
         }

   private:

   }; // end of class DLabeledWidget

#endif // DLABELEDWIDGET_H
