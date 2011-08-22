/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* The UI layout was based on the VLMC About dialog
* Copyright (C) 2008-2010 VideoLAN
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDate>
#include <QtCore/QFile>

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include "Config.h"
#include "core/Common.h"
#if !EDITOR
    #include "core/PluginLoader.h"
#endif

AboutDialog::AboutDialog(const Tano::AppType &type,
                         QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    if(type == Tano::Editor) {
        ui->labelIcon->setPixmap(QPixmap(":/logo/64x64/logo-editor.png"));
        ui->labelTitle->setText(ui->labelTitle->text().arg(tr("Editor"), Tano::version(), Tano::changeset()));
    } else {
        ui->labelTitle->setText(ui->labelTitle->text().arg(tr("Player"), Tano::version(), Tano::changeset()));
    }

    ui->labelBuild->setText(ui->labelBuild->text().arg(Tano::buildHostname(), Tano::buildSystem(), qVersion(), Tano::vlcQtVersionLibrary()));
    ui->labelCopyright->setText(ui->labelCopyright->text().arg(QDate::currentDate().toString("yyyy")));
    ui->labelBackendInfo->setText(ui->labelBackendInfo->text().arg(Tano::vlcQtVersionCore()));
    ui->labelBackendVersion->setText(ui->labelBackendVersion->text().arg(Tano::vlcQtVersionLibrary()));

    QFile file(":/info/AUTHORS");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->authors->setPlainText(QString::fromUtf8(file.readAll()));

#if EDITOR
    ui->tabWidget->removeTab(2);
#else
    QTreeWidgetItem *epg = new QTreeWidgetItem(ui->pluginsWidget);
    epg->setText(0, tr("EPG Plugins"));
    ui->pluginsWidget->setItemExpanded(epg, true);

    QFont font = epg->font(0);
    font.setBold(true);
    epg->setFont(0, font);

    PluginLoader *loader = new PluginLoader();
    for(int i = 0; i < loader->epgPlugin().size(); i++) {
        QTreeWidgetItem *pluginItem = new QTreeWidgetItem(epg);
        pluginItem->setText(0, loader->epgName()[i] + " (" + loader->epgFile()[i] + ")");
        pluginItem->setIcon(0, QIcon(":/icons/16x16/plugin.png"));
    }
    delete loader;
#endif

#if !WITH_EDITOR_VLCQT
    ui->tabWidget->removeTab(1);
#endif
}

AboutDialog::~AboutDialog()
{
    ui->pluginsWidget->clear();
    delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}
