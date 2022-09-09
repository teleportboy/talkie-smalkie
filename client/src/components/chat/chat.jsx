import React from "react";

import { ChatHeader } from "./header.jsx";
import { MessageArea } from "./message-area.jsx";
import { MessageInput } from "./message-input.jsx";
import { ChatList } from "../all-chats/chat-list.jsx";
import { SearchChatRoom } from "../chatroom-list/search-chatroom.jsx";
import { ChatRoomList } from "../chatroom-list/chatrooms-list.jsx";
import { CreateChatRoom } from "../all-chats/create-chatroom.jsx";
import { useSelector } from "react-redux";
import { Navigate } from "react-router-dom";

export function Chat() {
  const isLogged = useSelector(state => state.auth.isLogged);
  if (!isLogged) {
    return <Navigate to='/login'/>
  }

  return (
    <>      
      <ChatHeader />
      <MessageArea />
      <MessageInput />
      <CreateChatRoom/>
      <ChatList />
      <SearchChatRoom />
      <ChatRoomList />
    </>
  );
}