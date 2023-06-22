// [Legal]
// Copyright 2023 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QVariantList>
#include <QFutureSynchronizer>

class XmlParser : public QObject
{
  Q_OBJECT
public:
  explicit XmlParser(QObject *parent = nullptr);
  Q_INVOKABLE void parseXmlFileAsync(const QString& filePath);

signals:
  void xmlParseComplete(QVariantList parsedXmlElements);

private:
  void parseXmlFileInternal(const QString& filePath);
  QFutureSynchronizer<void> m_synchronizer;
};

#endif // XMLPARSER_H
