import React from "react";
import { useSelector } from "react-redux";
import { ChatRoomItem } from "./chatroom-item.jsx";


export function ChatRoomList() {
  const searchedRooms = useSelector(state => state.searchChatRooms.chatrooms);
  console.log(searchedRooms);

  return (
    <div className="item chatrooms-menu">chatrooms
      <ul>
        {
          searchedRooms.map((room, id) => {
            return <ChatRoomItem name={room} key={id}/>
          })
        }
      </ul>
    </div>
  )
}