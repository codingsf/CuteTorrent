﻿/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TOR_MANAGER_H
#define TOR_MANAGER_H
#include <vector>
#include <string>

#include "libtorrent/error_code.hpp"
#include "libtorrent/session_settings.hpp"
#include "libtorrent/torrent_handle.hpp"
#include "libtorrent/version.hpp"
#include "libtorrent/upnp.hpp"
#ifdef Q_WS_WIN
#pragma warning (disable: 4005)
#pragma warning (disable: 4100)
#pragma warning (disable: 4267)
#endif
#include <boost/bind.hpp>

#include "libtorrent/alert_types.hpp"
#include "libtorrent/session.hpp"

using boost::bind;
using namespace libtorrent;


#ifdef _WIN32

#if defined(_MSC_VER)
#	define for if (false) {} else for
#endif


#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#endif


#include "QApplicationSettings.h"
#include "TorrentStorrage.h"
#include "defs.h"
#include "ServiceCommon.h"
class QApplicationSettings;
class QTorrentDisplayModel;
class Torrent;
class TorrentStorrage;
namespace libtorrent
{
class alert;
}  // namespace libtorrent
struct openmagnet_info;
struct opentorrent_info;
class MessageBox;


class TorrentManager : public QObject
{
	Q_OBJECT
signals:
	void AddTorrentGui(Torrent*);
	void Notify(int, QString, QVariant);
	void initCompleted();
	void TorrentRemove(QString);
	void OnNewFeed();
	void OnFeedDeleted();
	void OnNewFeedItem();
	void OnFeedChanged();
protected:
	bool MoveFiles(QString oldStyleDirPath, QString newStyleDirPath);
	TorrentManager();
	~TorrentManager();
	static TorrentManager* _instance;
	static int _instanceCount;


private:
#if LIBTORRENT_VERSION_NUM < 100000
    upnp* m_pUpnp;
#endif
	void handle_alert(alert*);
	void writeSettings();
	TorrentStorrage* m_pTorrentStorrage;
	session* m_pTorrentSession;
	QApplicationSettings* m_pTorrentSessionSettings;
	int save_file(std::string const& filename, std::vector<char>& v);
	QString DTInstallPath;
	int max_connections_per_torrent;
	int listen_port;
	QString ipFilterFileName;
	bool useProxy;
	proxy_settings ps;
	NotificationSystemPtr m_pNotificationSys;
public slots:
	void RemoveTorrent(QString InfoHash, bool delFiles = false);
public:
	void SaveSession();
	void RefreshExternalPeerSettings();
	void InitSession();
	session_settings readSettings();
	pe_settings readEncSettings();
	void updateEncSettings(const pe_settings& settings);
	void updateSettings(const session_settings& settings);
	QString GetSessionDownloadSpeed();
	QString GetSessionUploadSpeed();
	QString GetSessionDownloaded();
	QString GetSessionUploaded();
	QString GetSessionDHTstate();
	static TorrentManager* getInstance();
	static void freeInstance();
	std::vector<torrent_status> GetTorrents();
	opentorrent_info* GetTorrentInfo(QString filename, error_code& ec);
	openmagnet_info* GetTorrentInfo(const torrent_handle& handle);
	
	bool AddMagnet(torrent_handle h, QString SavePath, QString group = "", QMap< QString, quint8> filepriorities = QMap<QString, quint8>());
	bool AddTorrent(QString path, QString name, QString save_path, error_code& ec, QMap<QString, quint8> filepriorities = QMap<QString, quint8>(), QString group = "", bool sequntial = false);
 #if LIBTORRENT_VERSION_NUM >= 10000
	void AddPortMapping(session::protocol_type type, ushort sourcePoert, ushort destPort);
#else
    void AddPortMapping(upnp::protocol_type type, ushort sourcePoert, ushort destPort);
#endif
	void PostTorrentUpdate();
	torrent_handle ProcessMagnetLink(QString link, error_code& ec);
	void CancelMagnetLink(QString link);
	void StartAllTorrents();
	void PauseAllTorrents();
	void SetUlLimit(int val);
	void SetDlLimit(int val);
	int GetDownloadLimit();
	int GetUploadLimit();
	Torrent* GetTorrentByInfoHash(const sha1_hash& hash);
	Torrent* GetTorrentByInfoHash(QString hash);
	void RereshPortForwardingSettings();
};
Q_DECLARE_METATYPE(opentorrent_info)

#endif
