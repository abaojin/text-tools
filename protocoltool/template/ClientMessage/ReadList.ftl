${""?left_pad(var_tab_count,"	")}var ${var_listCurrentName}Length:int = this.readUnsignedShort();
${""?left_pad(var_tab_count,"	")}for(var ${var_listCurrentName}Index:int = 0; ${var_listCurrentName}Index < ${var_listCurrentName}Length; ${var_listCurrentName}Index++){
<#assign var_tab_count = var_tab_count + 1/>
<#--引入写属性的模板-->
<#assign var_attributeDefType = var_listDefType/>
<#assign var_attributeName = 'var temp' + var_listCurrentName + 'Data' + ':*'/>
<#assign var_attributeLength = var_listChildLength/>
<#assign var_typeDef = var_typeDef/>
<#include "/ClientMessage/ReadAttribute.ftl">
${""?left_pad(var_tab_count,"	")}${var_listName}.push(temp${var_listCurrentName}Data);
<#assign var_listName = ''/>
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
