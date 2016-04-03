${""?left_pad(var_tab_count,"	")}short ${var_listCurrentName}Length = src.getShort();
${""?left_pad(var_tab_count,"	")}${var_listName} = new ${var_listType}[${var_listCurrentName}Length];
${""?left_pad(var_tab_count,"	")}for(int ${var_listCurrentName}Index = 0; ${var_listCurrentName}Index < ${var_listCurrentName}Length; ${var_listCurrentName}Index++){
<#assign var_tab_count = var_tab_count + 1/>
${""?left_pad(var_tab_count,"	")}${var_listType} temp${var_listCurrentName}Item = new ${var_listType}();
${""?left_pad(var_tab_count,"	")}${var_listName}[${var_listCurrentName}Index] = temp${var_listCurrentName}Item;
<#--引入写数据的模板-->
<#assign var_children = var_list.children/>
<#assign var_listNameTitle = var_listName + '[' + var_listCurrentName + 'Index]'/>
<#assign var_attributeNameTitle = 'temp' + var_listCurrentName + 'Item'/>
<#assign var_lastListName = var_listName/>
<#assign var_lastListCurrentName = var_listCurrentName/>
<#include "/ServerMessage/ReadByteArray.ftl">
<#assign var_listName = ''/>
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
