/*
 * Better Chat plugin for TeamSpeak 3
 * GPLv3 license
 *
 * Copyright (C) 2019 Luch (https://github.com/Luch00)
*/

#pragma once

#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include <QString>

constexpr short PATH_BUFSIZE = 512;
extern struct TS3Functions ts3Functions;
extern char returnCodeEmoteFileInfo[64];
extern char returnCodeEmoteFileRequest[64];
extern char pluginPath[PATH_BUFSIZE];
extern char configPath[PATH_BUFSIZE];

void logError(const QString& msg);
void logInfo(const QString& msg);
