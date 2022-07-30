import React from "react";
import { ChatListItem } from './chat-list-element.jsx';

export function ChatList() {
  return (
    <div className="item all-chats"> all chats
      <ul>
        <ChatListItem></ChatListItem>
        <ChatListItem></ChatListItem>
        <ChatListItem></ChatListItem>
      </ul>
    </div>

  )
}