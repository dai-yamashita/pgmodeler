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
\class RestricaoWidget
\brief Definição da classe que implementa o formulário de edição dos atributos de relacionamentos.
*/

#ifndef RELACIONAMENTO_WIDGET_H
#define RELACIONAMENTO_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_relacionamentowidget.h"
#include "tabelaobjetoswidget.h"

class RelacionamentoWidget: public BaseObjectWidget, public Ui::RelacionamentoWidget {
	private:
		Q_OBJECT
		/*! \brief Quantidade de elementos na lista de operações antes do relacionamento
			ser editado. Este atributo é usado para se saber, em caso de cancelamento
			da edição do relacionamento, a quantidade de operações relacionada ao
			objeto que necessitam ser removidas. Vide: cancelarConfiguracao() */
		unsigned qtd_operacoes;

		//! \brief Destacadores de sintaxe para os campos de nome da tabela de origem e destino
		SyntaxHighlighter *dest_tab_orig,
											*dest_tab_dest;

		//! \brief Tabela as quais armazenam os atributos e restrições do relacionamento
		TabelaObjetosWidget *tab_atributos,
												*tab_restricoes,
												*tab_objs_avancados;

		/*! \brief Lista os objetos do relacionamento na tabela respectiva, de acordo
			com o tipo do objeto passado */
		void listarObjetos(ObjectType tipo_obj);

		//! \brief Lista os objetos avançados
		void listarObjetosAvancados(void);

		//! \brief Exibe os dados de um objeto do relacionamento na lista específica de sua tabela
		void exibirDadosObjeto(TableObject *object, int idx_lin);

	protected:
		void setAttributes(DatabaseModel *model, OperationList *op_list, Table *tab_orig, Table *tab_dest, unsigned tipo_rel);

	public:
		RelacionamentoWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseRelationship *relacao);

	private slots:
		void hideEvent(QHideEvent *);

		//! \brief Adiciona um objeto   tabela a qual aciona o método
		void adicionarObjeto(void);

		//! \brief Edita um objeto selecionado  na tabela a qual aciona o método
		void editarObjeto(int idx_lin);

		//! \brief Remove um objeto selecionado  na tabela a qual aciona o método
		void removerObjeto(int idx_lin);

		//! \brief Remove todos os objetos da tabela a qual aciona o método
		void removerObjetos(void);

		//! \brief Exibe o formulário referente ao objeto criado ou que representa o relationamento
		void exibirObjetoAvancado(int idx);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);

		friend class ModeloWidget;
};

#endif
