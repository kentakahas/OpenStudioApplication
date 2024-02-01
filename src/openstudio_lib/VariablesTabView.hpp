/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2020-2023, OpenStudio Coalition and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef OPENSTUDIO_VARIABLESTABVIEW_HPP
#define OPENSTUDIO_VARIABLESTABVIEW_HPP

#include "MainTabView.hpp"
#include "../model_editor/QMetaTypes.hpp"
#include <openstudio/model/Model.hpp>
#include <openstudio/model/OutputVariable.hpp>
#include <openstudio/nano/nano_signal_slot.hpp>  // Signal-Slot replacement
#include <boost/optional.hpp>

class QCheckBox;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QLineEdit;

namespace openstudio {
class OSSwitch2;
class OSComboBox2;
class ProgressBarWithError;
class VariableListItem
  : public QWidget
  , public Nano::Observer
{
  Q_OBJECT;

 public:
  VariableListItem(const std::string& t_name, const std::string& t_keyValue, const boost::optional<openstudio::model::OutputVariable>& t_variable,
                   const openstudio::model::Model& t_model);

  virtual ~VariableListItem() {}

  bool matchesText(const QString& text, bool useRegex = false) const;

  // If the OSSwitch2 button is on
  bool isVariableEnabled() const;

  void setReportingFrequency(const std::string& freq);

 public slots:
  void setVariableEnabled(bool);

 private slots:
  void onOffClicked(bool);
  void indexChanged(const QString& t_frequency);

 private:
  REGISTER_LOGGER("openstudio.VariableListItem");

  std::string m_name;
  std::string m_keyValue;
  boost::optional<openstudio::model::OutputVariable> m_variable;
  openstudio::model::Model m_model;

  OSComboBox2* m_combobox;
  OSSwitch2* m_onOffButton;
};

class VariablesList
  : public QWidget
  , public Nano::Observer
{
  Q_OBJECT;

 public:
  explicit VariablesList(openstudio::model::Model t_model);
  virtual ~VariablesList();

 private slots:
  void onAdded(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&);
  void onRemoved(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&);

  void allOnClicked();
  void allOffClicked();

  void enableAll(bool);
  void updateVariableList();

  void onSearchTextEdited(const QString& text);

  void applyFrequencyToAllVisibleClicked();

 private:
  REGISTER_LOGGER("openstudio.VariablesList");
  openstudio::model::Model m_model;

  QLineEdit* m_searchBox;
  QCheckBox* m_searchUseRegex;
  QPushButton* m_displayAllBtn;
  QPushButton* m_displayOnlyEnabledBtn;
  QPushButton* m_displayOnlyDisabledBtn;
  QPushButton* m_allOnBtn;
  QPushButton* m_allOffBtn;
  QComboBox* m_frequencyComboBox;
  QPushButton* m_applyFrequencyBtn;
  ProgressBarWithError* m_progressBar;
  QVBoxLayout* m_listLayout;

  bool m_dirty;
  bool m_searchActive;
  QString m_searchText;
  std::vector<VariableListItem*> m_variables;
};

class VariablesTabView : public MainTabView
{
  Q_OBJECT;

 public:
  explicit VariablesTabView(openstudio::model::Model t_model, QWidget* parent = nullptr);

  virtual ~VariablesTabView() {}

 private:
  REGISTER_LOGGER("openstudio.VariablesTabView");
};

}  // namespace openstudio

#endif  // OPENSTUDIO_VARIABLESTABVIEW_HPP
