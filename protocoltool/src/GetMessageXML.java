import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;

import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import vo.MessageContentVO;
import vo.MessageVO;

public class GetMessageXML {

	public static String MESSAGE_FOLDER_NAME = "message";

	public void getMessageXMLInCurrentPath() throws Exception {
		File folder = new File("./" + MESSAGE_FOLDER_NAME).getCanonicalFile();
		System.out.println("folder of path : " + folder.getCanonicalPath());
		File[] files = folder.listFiles();
		if (files == null) {
			throw new Exception("no such path :" + folder.getCanonicalPath());
		} else {
			this.parseXML(files);
		}
	}

	private void parseXML(File[] files) throws Exception {
		for (int i = 0; i < files.length; i++) {
			File currentFile = (File) files[i];
			if (!currentFile.isFile()) {
				continue;
			} else {
				SAXReader reader = new SAXReader();
				Document document = reader.read(currentFile.getCanonicalPath());
				this.parse(currentFile, document);
			}
		}

		OutputFile outputFile = new OutputFile();
		outputFile.start();
	}

	@SuppressWarnings("unchecked")
	private void parse(File currentFile, Document doc) throws Exception {
		Element root = doc.getRootElement();
		for (Iterator<Element> i = root.elementIterator(); i.hasNext();) {
			Element element = (Element) i.next();
			MessageVO messageVO = new MessageVO();
			int messageType = 0;
			if (element.getQName().getName().equals(MessageType.SEND_MESSAGE)) {
				messageType = MessageType.SEND_MESSAGE_ID;
			} else if (element.getQName().getName().equals(MessageType.BACK_MESSAGE)) {
				messageType = MessageType.BACK_MESSAGE_ID;
			} else {
				throw new Exception("no such type: " + currentFile.getCanonicalPath() + "\n" + element.getQName().getName());
			}
			messageVO.setMessageTypeID(messageType);
			messageVO.setId(Integer.parseInt(element.attribute(MessageType.ID).getValue()));
			messageVO.setType(element.attribute(MessageType.TYPE).getValue().toUpperCase());
			System.out.println("message type: " + messageVO.getType());
			messageVO.setName(StringUtil.toUpperCaseFirstChart(element.attribute(MessageType.NAME).getValue() + MessageType.MESSAGE_CLASS_LAST_STRING));
			messageVO.setComment(element.attribute(MessageType.COMMENT).getValue());
			messageVO.setHandler(element.attribute(MessageType.NAME).getValue());
			if (!(element.attribute(MessageType.IS_AUTO_CREATE) == null)) {
				messageVO.setIsAutoCreate(Integer.parseInt(element.attribute(MessageType.IS_AUTO_CREATE).getValue()));
			}
			for (Iterator<Element> e = element.elementIterator(); e.hasNext();) {
				Element elementItem = (Element) e.next();
				messageVO.getChildren().add(this.parseMessageContentVO(elementItem));
			}
			DataManager.getInstance().getMessages().add(messageVO);
			if (messageVO.getMessageTypeID() == MessageType.SEND_MESSAGE_ID) {
				DataManager.getInstance().getSendMessages().add(messageVO);
			}
			if (messageVO.getMessageTypeID() == MessageType.BACK_MESSAGE_ID) {
				DataManager.getInstance().getBackMessages().add(messageVO);
			}
		}
	}

	@SuppressWarnings("unchecked")
	private MessageContentVO parseMessageContentVO(Element element) {
		MessageContentVO messageContentVO = new MessageContentVO();
		messageContentVO.setContentType(element.getQName().getName().toLowerCase());
		messageContentVO.setName(element.attributeValue(MessageType.NAME));
		if (!(element.attribute(MessageType.LENGTH) == null)) {
			messageContentVO.setLength(element.attributeValue(MessageType.LENGTH));
		}
		if (!(element.attribute(MessageType.COMMENT) == null)) {
			messageContentVO.setComment(element.attributeValue(MessageType.COMMENT));
		}
		if (element.hasContent()) {
			messageContentVO.setHasChildren(true);
			messageContentVO.setChildren(new ArrayList<MessageContentVO>());
			for (Iterator<Element> e = element.elementIterator(); e.hasNext();) {
				Element elementItem = (Element) e.next();
				messageContentVO.getChildren().add(this.parseMessageContentVO(elementItem));
			}
		}
		if (messageContentVO.getContentType().equals(MessageType.OBJECT) || (messageContentVO.getContentType().equals(MessageType.List) && messageContentVO.getHasChildren())) {
			messageContentVO.setType(StringUtil.toUpperCaseFirstChart(element.attributeValue(MessageType.TYPE)));
		} else {
			messageContentVO.setDefType(element.attributeValue(MessageType.TYPE).toLowerCase());
			messageContentVO.setType(TypeUtil.parseDefTypeToRealType(messageContentVO.getDefType()));
			messageContentVO.setServerType(TypeUtil.parseDefTypeToRealServerType(messageContentVO.getDefType()));
			messageContentVO.setJavaType(TypeUtil.parseDefTypeToJavaServerType(messageContentVO.getDefType()));
		}
		if (messageContentVO.getContentType().equals(MessageType.OBJECT)) {
			DataManager.getInstance().addObject(messageContentVO);
		}
		if(messageContentVO.getContentType().equals(MessageType.List) && messageContentVO.getHasChildren()){
			DataManager.getInstance().addObject(messageContentVO);
		}
			
		return messageContentVO;
	}
}
