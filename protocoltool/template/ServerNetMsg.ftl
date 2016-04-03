/********************************************************************
* Copyright (c) 2011,畅游天下WebGame事业部 All rights reserved.
* 
* FileName：KZ3NetMsg_${fileType}.h
* Summary：
* 
* Version：1.0
* Author：yunmiao
* Date：2011年9月20日   9:37
*********************************************************************/

#ifndef KZ3_NET_MSG_DEF
#error "Must define KZ3_NET_MSG_DEF before to include KZ3NetMsg_${fileType}.h"
#endif

<#list messages as message>
<#if fileType == 'C'>
<#if message.messageTypeID == 1>
KZ3_NET_MSG_DEF(${message.type})						// ${message.id},// ${message.comment}
</#if>
<#else>
<#if message.messageTypeID == 2>
KZ3_NET_MSG_DEF(${message.type})						// ${message.id},// ${message.comment}
</#if>
</#if>
</#list>


#undef KZ3_NET_MSG_DEF
