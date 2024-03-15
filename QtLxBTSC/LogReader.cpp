/*
 * Better Chat plugin for TeamSpeak 3
 * GPLv3 license
 *
 * Copyright (C) 2019 Luch (https://github.com/Luch00)
*/

#include "LogReader.h"
#include <QJsonArray>
#include <QJsonObject>
#include "globals.h"
#include <QRegularExpression>
#include <QBuffer>
#include <utils.h>

QJsonObject LogReader::readLog(const QString& serverUniqueID)
{
	QJsonObject obj;
	const QString chatsPath = QString("%1chats/%2").arg(configPath).arg(serverUniqueID);
	QByteArray log = readFile(QString("%1/server.html").arg(chatsPath));
	QJsonArray array = parseMessages(log);
	obj.insert("server", array);

	log = readFile(QString("%1/channel.html").arg(chatsPath));
	array = parseMessages(log);
	obj.insert("channel", array);
	return obj;
}

QJsonArray LogReader::readPrivateLog(const QString& serverUniqueID, const QString& clientUniqueID)
{
	const QString filePath = QString("%1chats/%2/clients/%3.html").arg(configPath, serverUniqueID, clientUniqueID);
	QByteArray log = readFile(filePath);
	QJsonArray array = parseMessages(log);
	return array;
}

QByteArray LogReader::readFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logInfo("LogReader: failed to open file");
		return "";
	}

	if (file.size() > maxBytesToRead)
	{
		file.seek(file.size() - maxBytesToRead);
	}

	QByteArray r = file.read(maxBytesToRead);
	file.close();
	return r;
}

QJsonArray LogReader::parseMessages(const QByteArray& log)
{
	const static QRegularExpression re(R"(&lt;(.*)&gt;.*(client://\d+/(.+)~(.+))\">.*TextMessage_Text\">(.*?)</span>)");

	QJsonArray array;
	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	buffer.write(log);
	buffer.seek(0);
	while (buffer.canReadLine())
	{
		QByteArray line = buffer.readLine();
		QRegularExpressionMatch match = re.match(line);
		if (match.hasMatch())
		{
			QJsonObject message;
			QString time = match.captured(1);
			QString link = match.captured(2);
			QString uid = match.captured(3);
			QString name = match.captured(4);
			QString text = match.captured(5);

			message.insert("time", time);
			message.insert("link", link);
			message.insert("uid", utils::ts3WeirdBase16(uid));
			message.insert("name", name);
			message.insert("text", text);
			array.append(message);
		}
	}
	return array;
}
