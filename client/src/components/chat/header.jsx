import React from "react";
import { useSelector } from 'react-redux';

export function ChatHeader() {
  const chat_id = useSelector(state => state.currentChat.id);
  return (
    <div className="item chat-menu">
      <span>chatroom#{chat_id}</span>
    </div>
  );
}