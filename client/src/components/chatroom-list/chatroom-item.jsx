import React from "react";
import { useDispatch, useSelector } from "react-redux";
import { addChatRoom } from "../../store/chatrooms-list-slice.js";
import { createChat } from "../../store/chat-area-slice.js";
import { store } from "../../store/store";
import { subscribeToChatRoom } from "../../sse-connection/conection.js";

export function ChatRoomItem({ name }) {
  const dispatch = useDispatch();
  const activeRooms = useSelector(state => state.chatRooms.roomsList);

  const joinChatRoom = (event) => {
    dispatch(addChatRoom(name));
    dispatch(createChat(name));

    fetch('/room_join', {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        chatroom: name,
        uname: store.getState().auth.uname
      })
    });
    subscribeToChatRoom(name, dispatch);
  }

  const isJoined = () => {
    return !!activeRooms.find(room => {
      return room === name;
    });
  }

  return (
    <span className="chatroom">
      <li className="item">{name}</li>
      <button disabled={isJoined()} onClick={joinChatRoom}>join</button>
    </span>
  );
}