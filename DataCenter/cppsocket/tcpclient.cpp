/***************************************************************************
 *   CPPSocket - library                                                   *
 *                                                                         *
 *   Copyright (C) 2001 by Ralf-Christian Juergensen                       *
 *                         <rcj@users.sourceforge.net>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be usefull, but  *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   Library General Public License for more details.                      *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston,                *
 *   MA 02111-1307, USA                                                    *
 ***************************************************************************/

#include "tcpclient.h"

namespace CPPSocket
{
  void TCPClient::connect(const Address& addr) throw (Exception)
  {
    socket.open(Socket::TCP);

    socket.connect(addr);
    
    setConnected(true);
  }

  void TCPClient::start_connect(const Address& addr) throw (Exception)
  {
    socket.open(Socket::TCP);
    socket.setblock(false);
    socket.connect(addr);
  }

  void TCPClient::connected(void)
  {
	  setConnected(true);
	  //socket.setblock(true);
  }

  Address TCPClient::getOwnAddress() throw (Exception)
  {
    return socket.getsockname();
  }

}
