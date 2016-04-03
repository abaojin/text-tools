${""?left_pad(var_tab_count,"	")}var ${var_listCurrentName}Length:int = this.readUnsignedShort();
${""?left_pad(var_tab_count,"	")}for(var ${var_listCurrentName}Index:int = 0; ${var_listCurrentName}Index < ${var_listCurrentName}Length; ${var_listCurrentName}Index++){
<#assign var_tab_count = var_tab_count + 1/>
${""?left_pad(var_tab_count,"	")}var temp${var_listCurrentName}Item:${var_listType} = new ${var_listType}();
${""?left_pad(var_tab_count,"	")}${var_listName}.push(temp${var_listCurrentName}Item);
<#assign var_oldListNameTitle = var_listNameTitle>
<#assign var_listName = 'temp' + var_listCurrentName + 'Item'/>
<#--引入写数据的模板-->
<#assign var_children = var_list.children/>
<#assign var_listNameTitle = var_listName + '.'/>
<#assign var_attributeNameTitle = 'temp' + var_listCurrentName + 'Item'/>
<#assign var_lastListName = var_listName/>
<#assign var_lastListCurrentName = var_listCurrentName/>
<#include "/ClientMessage/ReadByteArray.ftl">
<#assign var_listName = ''/>
<#assign var_listNameTitle = var_oldListNameTitle/>
<#assign var_tab_count = var_tab_count - 1/>
${""?left_pad(var_tab_count,"	")}}
