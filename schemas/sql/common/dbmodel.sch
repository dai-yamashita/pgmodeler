# SQL definition for database model
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[/*]
[ Database generated with pgModeler (PostgreSQL Database Modeler).
  Project Site: pgmodeler.com.br
  Model Author: ] 

%if @{author} %then
 @{author} 
%else
  ---
%end
[ */] $br $br


%if @{export-to-file} %then

 %if @{role} %then @{role} %end
 %if @{tablespace} %then 
   [/*  Tablespaces creation must be done outside an multicommand file.] $br
   [  These commands were put in this file only for convenience.] $br $br
    @{tablespace} $br
    */ $br $br
 %end

$br
    [/* Database creation must be done outside an multicommand file.] $br
    [   These commands were put in this file only for convenience.] $br $br
   @{database} $br
*/ $br $br
%end

%if @{schema} %then @{schema} %end
%if @{shell-types} %then @{shell-types} %end
%if @{objects} %then @{objects} %end
%if @{grant} %then @{grant} %end

$br
