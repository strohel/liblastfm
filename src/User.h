/*
   Copyright 2009-2010 Last.fm Ltd.
      - Primarily authored by Max Howell, Jono Cole, Doug Mansell and Michael Coffey

   This file is part of liblastfm.

   liblastfm is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   liblastfm is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with liblastfm.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef LASTFM_USER_H
#define LASTFM_USER_H

#include <QString>
#include <QStringList>
#include <QUrl>

#include "AbstractType.h"
#include "ws.h"

namespace lastfm
{
    class UserList;

    class LASTFM_DLLEXPORT Gender
    {
        QString s;

    public:
        Gender() :s(/*confused!*/){}

        Gender( const QString& ss ) :s( ss.toLower() )
        {}

        bool known() const { return male() || female(); }
        bool male() const { return s == "m"; }
        bool female() const { return s == "f"; }

        QString toString() const
        {
            QString result;

            if (male())
                result = QObject::tr( "m" );
            else if (female())
                result = QObject::tr( "f" );
            else
                result = QObject::tr( "n" ); // as in neuter

            return result;
        }
    };

    class LASTFM_DLLEXPORT User : public AbstractType
    {
    public:
        enum Type
        {
            TypeUser,
            TypeSubscriber,
            TypeModerator,
            TypeStaff,
            TypeAlumni
        };

    public:
        User() : AbstractType(), m_name( lastfm::ws::Username ), m_match( -1.0f )
        {}

        User( const QString& name ) : AbstractType(), m_name( name ), m_match( -1.0f )
        {}

        User( const class XmlQuery& xml );

        lastfm::User& operator=( const lastfm::User& that );
        bool operator==(const lastfm::User& that) const { return m_name == that.m_name; }
        bool operator<(const lastfm::User& that) const { return m_name < that.m_name; }

        operator QString() const { return m_name; }

        QString name() const { return m_name; }
        void setName( const QString& name ){ m_name = name; }

        Type type() const { return m_type;}
        void setType( Type type ){ m_type = type; }

        bool isSubscriber() const{ return m_isSubscriber; }
        void setIsSubscriber( bool subscriber );

        bool canBootstrap() const{ return m_canBootstrap; }
        void setCanBootstrap( bool canBootstrap );

        quint32 scrobbleCount() const{ return m_scrobbles; }
        void setScrobbleCount( quint32 scrobblesCount );

        QDateTime dateRegistered() const { return m_registered; }
        void setDateRegistered( const QDateTime& date );

        Gender gender() const { return m_gender; }
        QString country() const { return m_country; }

        QString realName() const { return m_realName; }
        void setRealName( const QString& realName );

        QUrl imageUrl( ImageSize size = Large, bool square = false ) const;
        void setImages( const QList<QUrl>& images );

        void setAge( unsigned short age );

        void setGender( const QString& s );
        void setCountry( const QString& country );
    
        /** use Tag::list() on the response to get a WeightedStringList */
        QNetworkReply* getTopTags() const;

        /** use User::list() on the response to get a QList<User> */
        QNetworkReply* getFriends(  bool recentTracks = false, int limit = 50, int page = 1 ) const;
        QNetworkReply* getFriendsListeningNow( int limit = 50, int page = 1 ) const;
        QNetworkReply* getNeighbours( int limit = 50, int page = 1 ) const;
    
        QNetworkReply* getPlaylists() const;
        QNetworkReply* getTopArtists( QString period = "overall", int limit = 50, int page = 1 ) const;
        QNetworkReply* getRecentTracks( int limit = 50, int page = 1 ) const;
        QNetworkReply* getRecentArtists() const;
        QNetworkReply* getRecentStations(  int limit = 10, int page = 1  ) const;
        QNetworkReply* getRecommendedArtists( int limit = 50, int page = 1 ) const;

        /** you can only get information about the any user */
        static QNetworkReply* getInfo( const QString& username = lastfm::ws::Username );

        /** a verbose string, eg. "A man with 36,153 scrobbles" */
        QString getInfoString() const;
    
        static UserList list( QNetworkReply* );

        QString toString() const { return name(); }
        QDomElement toDomElement( QDomDocument& ) const { return QDomElement(); }       
    
        /** the user's profile page at www.last.fm */
        QUrl www() const;
    
        /** Returns the match between the logged in user and the user which this
          * object represents (if < 0.0f then not set) */
        float match() const { return m_match; }
    
    protected:        
        QMap<QString, QString> params( const QString& method ) const;

    protected:
        QString m_name;
        Type m_type;
        QList<QUrl> m_images;
        float m_match;
        QString m_realName;
        Gender m_gender;
        unsigned short m_age;
        unsigned int m_scrobbles;
        QDateTime m_registered;
        QString m_country;
        bool m_isSubscriber;
        bool m_canBootstrap;
    };

    class LASTFM_DLLEXPORT UserList : public QList<User>
    {
    public:
        int total;
        int page;
        int perPage;
        int totalPages;
    };
}

#endif
