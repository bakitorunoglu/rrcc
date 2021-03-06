#include "zones.h"

zonesDialog::zonesDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);
	setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

	foreach(CLEANZONE zone, reinterpret_cast<MainWindow*>(parent)->cfg.zones)
	{
		comboBox_zone->addItem(zone.label);
	}

	foreach(GOTO gotO, reinterpret_cast<MainWindow*>(parent)->cfg.gotos)
	{
		comboBox_goto->addItem(gotO.label);
	}

	if(!comboBox_zone->count())
	{
		tabWidget->setTabEnabled(0, false);
		tabWidget->setCurrentIndex(1);
	}

	if(!comboBox_goto->count())
	{
		tabWidget->setTabEnabled(1, false);
	}

	buttonBox->button(QDialogButtonBox::Discard)->setText(tr("Delete"));

	emit on_tabWidget_currentChanged(tabWidget->currentIndex());
}

void zonesDialog::on_tabWidget_currentChanged(int index)
{
	if(index)
	{
		emit reinterpret_cast<MainWindow*>(parent())->aboutToHideZone();
		emit reinterpret_cast<MainWindow*>(parent())->hoveredGoto(reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->actions().at(comboBox_goto->currentIndex()));

        emit on_comboBox_goto_currentIndexChanged(comboBox_goto->currentIndex());
    }
	else
	{
		emit reinterpret_cast<MainWindow*>(parent())->aboutToHideGoto();
		emit reinterpret_cast<MainWindow*>(parent())->hoveredZone(reinterpret_cast<MainWindow*>(parent())->menu_map_zones->actions().at(comboBox_zone->currentIndex()));

        emit on_comboBox_zone_currentIndexChanged(comboBox_zone->currentIndex());
    }
}

// tab zone

void zonesDialog::on_comboBox_zone_currentIndexChanged(int index)
{
	label_current->setText(tr("Zone %1 / %2").arg(index + 1).arg(reinterpret_cast<MainWindow*>(parent())->cfg.zones.count()));

	spinBox_x1->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.zones.at(index).x1);
	spinBox_y1->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.zones.at(index).y1);
	spinBox_x2->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.zones.at(index).x2);
	spinBox_y2->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.zones.at(index).y2);
	spinBox_times->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.zones.at(index).times);

	emit reinterpret_cast<MainWindow*>(parent())->hoveredZone(reinterpret_cast<MainWindow*>(parent())->menu_map_zones->actions().at(index));
}

void zonesDialog::zonePositionChanged()
{
	reinterpret_cast<MainWindow*>(parent())->zone_preview_rect = QRect(static_cast<int>((reinterpret_cast<MainWindow*>(parent())->cfg.swap_x ? spinBox_x1->value() : MAPSIZE - spinBox_x2->value()) / MAPFACTOR), static_cast<int>((reinterpret_cast<MainWindow*>(parent())->cfg.swap_y ? spinBox_y1->value() : MAPSIZE - spinBox_y2->value()) / MAPFACTOR), static_cast<int>(qAbs(spinBox_x1->value() - spinBox_x2->value()) / MAPFACTOR), static_cast<int>(qAbs(spinBox_y1->value() - spinBox_y2->value()) / MAPFACTOR));

	if(reinterpret_cast<MainWindow*>(parent())->zone_preview_item)
	{
		reinterpret_cast<MainWindow*>(parent())->scene->removeItem(reinterpret_cast<MainWindow*>(parent())->zone_preview_item);
	}

	reinterpret_cast<MainWindow*>(parent())->zone_preview_item = reinterpret_cast<MainWindow*>(parent())->scene->addRect(reinterpret_cast<MainWindow*>(parent())->zone_preview_rect, QPen(Qt::green, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin), QBrush(QColor(0, 255, 0, 64)));
}

void zonesDialog::on_spinBox_x1_valueChanged(__attribute__((unused)) int val)
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_y1_valueChanged(__attribute__((unused)) int val)
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_x2_valueChanged(__attribute__((unused)) int val)
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_y2_valueChanged(__attribute__((unused)) int val)
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_x1_editingFinished()
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_y1_editingFinished()
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_x2_editingFinished()
{
	zonePositionChanged();
}

void zonesDialog::on_spinBox_y2_editingFinished()
{
	zonePositionChanged();
}

// tab goto

void zonesDialog::on_comboBox_goto_currentIndexChanged(int index)
{
	spinBox_x->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.gotos.at(index).x);
	spinBox_y->setValue(reinterpret_cast<MainWindow*>(parent())->cfg.gotos.at(index).y);

	emit reinterpret_cast<MainWindow*>(parent())->hoveredGoto(reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->actions().at(index));
}

void zonesDialog::gotoPositionChanged()
{
	reinterpret_cast<MainWindow*>(parent())->pos_flag = QPointF((reinterpret_cast<MainWindow*>(parent())->cfg.swap_x ? spinBox_x->value() : MAPSIZE - spinBox_x->value()) / MAPFACTOR, (reinterpret_cast<MainWindow*>(parent())->cfg.swap_y ? spinBox_y->value() : MAPSIZE - spinBox_y->value()) / MAPFACTOR);

	reinterpret_cast<MainWindow*>(parent())->drawFlags(false, true);
	reinterpret_cast<MainWindow*>(parent())->drawFlags(true, true);
}

void zonesDialog::on_spinBox_x_valueChanged(__attribute__((unused)) int val)
{
	gotoPositionChanged();
}

void zonesDialog::on_spinBox_y_valueChanged(__attribute__((unused)) int val)
{
	gotoPositionChanged();
}

void zonesDialog::on_spinBox_x_editingFinished()
{
	gotoPositionChanged();
}

void zonesDialog::on_spinBox_y_editingFinished()
{
	gotoPositionChanged();
}

void zonesDialog::reject()
{
	emit reinterpret_cast<MainWindow*>(parent())->aboutToHideZone();
	emit reinterpret_cast<MainWindow*>(parent())->aboutToHideGoto();

	accept();
}

void zonesDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	int index_zone = comboBox_zone->currentIndex();
	int index_goto = comboBox_goto->currentIndex();

	if(!button->hasFocus())
	{
		return;
	}

	if(buttonBox->standardButton(button) == QDialogButtonBox::Save)
	{
		if(tabWidget->currentWidget()->objectName() == "tab_zone")
		{
			if(comboBox_zone->currentText().isEmpty())
			{
				QMessageBox::warning(this, APPNAME, tr("Please enter a label for this zone!"));
			}
			else
			{
				comboBox_zone->setItemText(index_zone, comboBox_zone->currentText());

				reinterpret_cast<MainWindow*>(parent())->menu_map_zones->actions().at(index_zone)->setText(comboBox_zone->currentText());
				reinterpret_cast<MainWindow*>(parent())->menu_map_zones->actions().at(index_zone)->setStatusTip(tr("Clean Zone %1").arg(comboBox_zone->currentText()));

				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].label = comboBox_zone->currentText();
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].x1 = spinBox_x1->value();
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].y1 = spinBox_y1->value();
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].x2 = spinBox_x2->value();
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].y2 = spinBox_y2->value();
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.data()[index_zone].times = spinBox_times->value();
			}
		}
		else if(tabWidget->currentWidget()->objectName() == "tab_goto")
		{
				if(comboBox_goto->currentText().isEmpty())
				{
					QMessageBox::warning(this, APPNAME, tr("Please enter a label for this goto!"));
				}
				else
				{
					comboBox_goto->setItemText(index_goto, comboBox_goto->currentText());

					reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->actions().at(index_goto)->setText(comboBox_goto->currentText());
					reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->actions().at(index_goto)->setStatusTip(tr("Goto %1").arg(comboBox_goto->currentText()));

					reinterpret_cast<MainWindow*>(parent())->cfg.gotos.data()[index_goto].label = comboBox_goto->currentText();
					reinterpret_cast<MainWindow*>(parent())->cfg.gotos.data()[index_goto].x = spinBox_x->value();
					reinterpret_cast<MainWindow*>(parent())->cfg.gotos.data()[index_goto].y = spinBox_y->value();
				}
		}
	}
	else if(buttonBox->standardButton(button) == QDialogButtonBox::Discard)
	{
		if(tabWidget->currentWidget()->objectName() == "tab_zone")
		{
			if(QMessageBox::question(this, APPNAME, tr("Really delete zone \"%1\"?").arg(comboBox_zone->currentText()), QMessageBox::Yes | QMessageBox::No,  QMessageBox::Yes) ==  QMessageBox::Yes)
			{
				reinterpret_cast<MainWindow*>(parent())->cfg.zones.removeAt(index_zone);
				reinterpret_cast<MainWindow*>(parent())->menu_map_zones->removeAction(reinterpret_cast<MainWindow*>(parent())->menu_map_zones->actions().at(index_zone));

				if(!reinterpret_cast<MainWindow*>(parent())->cfg.zones.count())
				{
					if(reinterpret_cast<MainWindow*>(parent())->cfg.gotos.count())
					{
						tabWidget->setCurrentIndex(1);
						tabWidget->setTabEnabled(0, false);
					}
					else
					{
						close();
					}
				}
				else
				{
					comboBox_zone->removeItem(index_zone);
				}
			}
		}
		else if(tabWidget->currentWidget()->objectName() == "tab_goto")
		{
			if(QMessageBox::question(this, APPNAME, tr("Really delete goto \"%1\"?").arg(comboBox_goto->currentText()), QMessageBox::Yes | QMessageBox::No,  QMessageBox::Yes) ==  QMessageBox::Yes)
			{
				reinterpret_cast<MainWindow*>(parent())->cfg.gotos.removeAt(index_goto);
				reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->removeAction(reinterpret_cast<MainWindow*>(parent())->menu_map_gotos->actions().at(index_goto));

				if(!reinterpret_cast<MainWindow*>(parent())->cfg.gotos.count())
				{
					if(reinterpret_cast<MainWindow*>(parent())->cfg.zones.count())
					{
						tabWidget->setCurrentIndex(0);
						tabWidget->setTabEnabled(1, false);
					}
					else
					{
						close();
					}
				}
				else
				{
					comboBox_goto->removeItem(index_goto);
				}
			}
		}
	}
	else if(buttonBox->standardButton(button) == QDialogButtonBox::Close)
	{
		close();
	}
}
