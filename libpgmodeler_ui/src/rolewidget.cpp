#include "rolewidget.h"
#include "visaoobjetoswidget.h"
extern VisaoObjetosWidget *selecaoobjetos_wgt;

RoleWidget::RoleWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_ROLE)
{
	TabelaObjetosWidget *obj_tab=NULL;
	QGridLayout *grid=NULL;
	unsigned i;

	Ui_RoleWidget::setupUi(this);
	configureFormLayout(role_grid, OBJ_ROLE);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(members_twg, SIGNAL(currentChanged(int)), this, SLOT(configureRoleSelection(void)));

	//Alocation of the member role tables
	for(i=0; i < 3; i++)
	{
		obj_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																		TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
		members_tab[i]=obj_tab;

		obj_tab->definirNumColunas(6);

		obj_tab->definirRotuloCabecalho(trUtf8("Role"),0);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),0);

		obj_tab->definirRotuloCabecalho(trUtf8("SysID"),1);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/uid.png"),1);

		obj_tab->definirRotuloCabecalho(trUtf8("Validity"),2);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/validade.png"),2);

		obj_tab->definirRotuloCabecalho(trUtf8("Member of"),3);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),3);

		obj_tab->definirRotuloCabecalho(trUtf8("Members"),4);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),4);

		obj_tab->definirRotuloCabecalho(trUtf8("Members (Admin.)"),5);
		obj_tab->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),5);

		grid=new QGridLayout;
		grid->addWidget(obj_tab,0,0,1,1);
		grid->setContentsMargins(2,2,2,2);
		members_twg->widget(i)->setLayout(grid);
	}

	parent_form->setMinimumSize(500, 530);
}

void RoleWidget::configureRoleSelection(void)
{
	unsigned i;

	//Disconnects all signals from the member role tables
	for(i=0; i < 3; i++)
		disconnect(members_tab[i],0,this,0);

	//Connects the signal/slots only on the current table
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_linhaAdicionada(int)), this, SLOT(selectMemberRole(void)));
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_linhaEditada(int)), this, SLOT(selectMemberRole(void)));
}

void RoleWidget::selectMemberRole(void)
{
	selecaoobjetos_wgt->definirObjetoVisivel(OBJ_ROLE, true);
	selecaoobjetos_wgt->definirModelo(this->model);
	selecaoobjetos_wgt->show();
}

void RoleWidget::hideEvent(QHideEvent *event)
{
	unsigned i;

	disconnect(selecaoobjetos_wgt,0,this,0);

	for(i=0; i < 3; i++)
		members_tab[i]->blockSignals(true);

	for(i=0; i < 3; i++)
	{
		members_tab[i]->removerLinhas();
		members_tab[i]->blockSignals(false);
	}

	members_twg->setCurrentIndex(0);
	sysid_sb->setValue(sysid_sb->minValue());
	passwd_edt->clear();
	conn_limit_sb->setValue(conn_limit_sb->minValue());
	superusr_chk->setChecked(false);
	inh_perm_chk->setChecked(false);
	create_db_chk->setChecked(false);
	can_login_chk->setChecked(false);
	create_user_chk->setChecked(false);
	encrypt_pass_chk->setChecked(false);

	BaseObjectWidget::hideEvent(event);
}

void RoleWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Role *role)
{
	if(role)
	{
		sysid_sb->setValue(role->getSysid());
		conn_limit_sb->setValue(role->getConnectionLimit());
		passwd_edt->setText(role->getPassword());
		validity_dte->setDateTime(QDateTime::fromString(role->getValidity(),"yyyy-MM-dd hh:mm"));
		superusr_chk->setChecked(role->getOption(Role::OP_SUPERUSER));
		create_db_chk->setChecked(role->getOption(Role::OP_CREATEDB));
		create_user_chk->setChecked(role->getOption(Role::OP_CREATEROLE));
		encrypt_pass_chk->setChecked(role->getOption(Role::OP_ENCRYPTED));
		inh_perm_chk->setChecked(role->getOption(Role::OP_INHERIT));
		can_login_chk->setChecked(role->getOption(Role::OP_LOGIN));
	}

	BaseObjectWidget::setAttributes(model, op_list, role);

	fillMembersTable();
	connect(selecaoobjetos_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedRoleData(void)));
	configureRoleSelection();
}

void RoleWidget::showRoleData(Role *role, unsigned table_id, unsigned row)
{
	if(role)
	{
		QString str_aux;
		Role *aux_role=NULL;
		unsigned count, i, type_id,
							role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		if(table_id > 3)
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		members_tab[table_id]->definirDadoLinha(QVariant::fromValue(reinterpret_cast<void *>(role)), row);
		members_tab[table_id]->definirTextoCelula(QString::fromUtf8(role->getName()), row, 0);
		members_tab[table_id]->definirTextoCelula(QString("%1").arg(role->getSysid()), row, 1);
		members_tab[table_id]->definirTextoCelula(role->getValidity(), row, 2);

		for(type_id=0; type_id < 3; type_id++)
		{
			count=role->getRoleCount(role_types[type_id]);

			for(i=0; i < count; i++)
			{
				aux_role=role->getRole(role_types[type_id], i);
				str_aux+=aux_role->getName();
				if(i < count-1) str_aux+=", ";
			}

			members_tab[table_id]->definirTextoCelula(QString::fromUtf8(str_aux), row, 3 + type_id);
			str_aux.clear();
		}
	}
}

void RoleWidget::fillMembersTable(void)
{
	if(this->object)
	{
		Role *aux_role=NULL, *role=NULL;
		unsigned count, i, type_id,
				role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		role=dynamic_cast<Role *>(this->object);

		for(type_id=0; type_id < 3; type_id++)
		{
			count=role->getRoleCount(role_types[type_id]);
			members_tab[type_id]->blockSignals(true);

			for(i=0; i < count; i++)
			{
				aux_role=role->getRole(role_types[type_id], i);
				members_tab[type_id]->adicionarLinha();
				showRoleData(aux_role, type_id, i);
			}

			members_tab[type_id]->blockSignals(true);
			members_tab[type_id]->limparSelecao();
		}
	}
}

void RoleWidget::showSelectedRoleData(void)
{
	unsigned idx_tab;
	int lin, idx_lin=-1;
	BaseObject *obj_sel=NULL;

	//Get the selected role
	obj_sel=selecaoobjetos_wgt->obterObjetoSelecao();

	//Gets the index of the table where the role data is displayed
	idx_tab=members_twg->currentIndex();
	lin=members_tab[idx_tab]->obterLinhaSelecionada();

	if(obj_sel)
		idx_lin=members_tab[idx_tab]->obterIndiceLinha(QVariant::fromValue<void *>(dynamic_cast<void *>(obj_sel)));

	//Raises an error if the user try to assign the role as member of itself
	if(obj_sel && obj_sel==this->object)
	{
		throw Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
										.arg(QString::fromUtf8(obj_sel->getName()))
										.arg(QString::fromUtf8(name_edt->text())),
										ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//If the role does not exist on table, show its data
	else if(obj_sel && idx_lin < 0)
		showRoleData(dynamic_cast<Role *>(obj_sel), idx_tab, lin);
	else
	{
		/* If the current row does not has a value indicates that it is recently added and does not have
			 data, in this case it will be removed */
		if(!members_tab[idx_tab]->obterDadoLinha(lin).value<void *>())
			members_tab[idx_tab]->removerLinha(lin);

		//Raises an error if the role already is in the table
		if(obj_sel && idx_lin >= 0)
		{
			throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
											.arg(QString::fromUtf8(obj_sel->getName()))
											.arg(QString::fromUtf8(name_edt->text())),
											ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
}

void RoleWidget::applyConfiguration(void)
{
	Role *role=NULL, *aux_role=NULL;
	unsigned count, i, type_id,
			role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

	try
	{
		startConfiguration<Role>();

		role=dynamic_cast<Role *>(this->object);
		role->setSysid(sysid_sb->value());
		role->setConnectionLimit(conn_limit_sb->value());
		role->setPassword(passwd_edt->text());
		role->setValidity(validity_dte->dateTime().toString("yyyy-MM-dd hh:mm"));

		role->setOption(Role::OP_SUPERUSER, superusr_chk->isChecked());
		role->setOption(Role::OP_CREATEDB, create_db_chk->isChecked());
		role->setOption(Role::OP_CREATEROLE, create_user_chk->isChecked());
		role->setOption(Role::OP_ENCRYPTED, encrypt_pass_chk->isChecked());
		role->setOption(Role::OP_INHERIT, inh_perm_chk->isChecked());
		role->setOption(Role::OP_LOGIN, can_login_chk->isChecked());

		for(type_id=0; type_id < 3; type_id++)
		{
			count=members_tab[type_id]->obterNumLinhas();
			if(count > 0) role->removeRoles(role_types[type_id]);

			for(i=0; i < count; i++)
			{
				aux_role=reinterpret_cast<Role *>(members_tab[type_id]->obterDadoLinha(i).value<void *>());
				role->addRole(role_types[type_id], aux_role);
			}
		}

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

