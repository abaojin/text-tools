${""?left_pad(var_tab_count,"	")}this.writeShort(${var_listName}.length);
${""?left_pad(var_tab_count,"	")}for each(var ${var_listCurrentName}Item:${var_listType} in ${var_listName}){
<#assign var_tab_count = var_tab_count + 1/>
<#--引入写属性的模板-->
<#assign var_attributeDefType = var_listDefType/>
<#assign var_attributeName = var_listCurrentName + 'Item'/>
<#assign var_attributeLength = var_listChildLength/>
<#assign var_typeDef = var_typeDef/>
<#include "/ClientMessage/WriteAttribute.ftl">
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
