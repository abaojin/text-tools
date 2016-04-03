${""?left_pad(var_tab_count,"	")}src.putShort((short)${var_listName}.length);
${""?left_pad(var_tab_count,"	")}for(int ${var_listCurrentName}Index = 0; ${var_listCurrentName}Index < ${var_listName}.length; ${var_listCurrentName}Index++){
<#assign var_tab_count = var_tab_count + 1/>
<#assign var_old_listNameTitle = var_listNameTitle>
<#--引入写数据的模板-->
<#assign var_children = var_list.children/>
<#assign var_listNameTitle = var_listName + '[' + var_listCurrentName + 'Index]'/>
<#assign var_attributeNameTitle = var_listName + '[' + var_listCurrentName + 'Index]'/>
<#include "/ServerMessage/WriteByteArray.ftl">
<#assign var_listName = 'this'/>
<#assign var_listNameTitle = var_old_listNameTitle/>
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
