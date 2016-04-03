${""?left_pad(var_tab_count,"	")}this.writeShort(${var_listName}.length);
${""?left_pad(var_tab_count,"	")}for each(var ${var_listCurrentName}Item:${var_listType} in ${var_listName}){
<#assign var_tab_count = var_tab_count + 1/>
<#--引入写数据的模板-->
<#assign var_children = var_list.children/>
<#assign var_listNameTitle = var_listCurrentName + 'Item'/>
<#assign var_listName = var_listNameTitle/>
<#assign var_attributeNameTitle = var_listCurrentName + 'Item'/>
<#include "/ClientMessage/WriteByteArray.ftl">
<#assign var_listName = 'this'/>
<#assign var_listNameTitle = 'this'/>
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
