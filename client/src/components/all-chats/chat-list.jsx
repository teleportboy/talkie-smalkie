import React from "react";
import { useSelector } from "react-redux";
import { ChatListItem } from './chat-list-element.jsx';

export function ChatList() {  
  const chatRooms = useSelector((state) => state.chatRooms);  
  
  return (
    <div className="item all-chats"> all chatrooms
      <ul>
        {
          chatRooms.roomsList.map((chatRoom, id) => {
            return <ChatListItem chat_id={chatRoom} key={id}/>;
          })
        }
      </ul>
    </div>
  );
}