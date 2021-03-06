/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libpgmodeler_ui
\class FormExportacao
\brief Formulário de exportação do modelo
*/

#ifndef FORM_EXPORTACAO_H
#define FORM_EXPORTACAO_H

#include "ui_formexportacao.h"
#include "schemaparser.h"
#include "modelowidget.h"

class FormExportacao: public QDialog, public Ui::FormExportacao {
	private:
		Q_OBJECT
		ModeloWidget *modelo_wgt;

	public:
		FormExportacao(QWidget * parent = 0, Qt::WindowFlags f = 0);

	public slots:
		void show(ModeloWidget *modelo);
		void hideEvent(QHideEvent *);

	private slots:
		void habilitarTipoExportacao(void);
		void exportarModelo(void);
		void selecionarArquivoDestino(void);
		void ocultarProgressoExportacao(void);
};

#endif
