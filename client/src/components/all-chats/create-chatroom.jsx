import React from "react";
import { useDispatch } from "react-redux";
import { addChatRoom } from "../../store/chatrooms-list-slice";
import { createChat } from "../../store/chat-area-slice";
import { store } from "../../store/store";
import { subscribeToChatRoom } from "../../sse-connection/conection.js";

export function CreateChatRoom() {
  const dispatch = useDispatch();
  
  const createChatRoom = () => {
    const roomName = document.getElementById('create_chatroom').value;

    fetch('/chatroom', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        chatroom: roomName
      })
    })
    .then(response => response.json())
    .then(data => {
      if (data.isOk === 'true') {
        dispatch(addChatRoom(roomName));
        dispatch(createChat(roomName));
        subscribeToChatRoom(roomName, dispatch);
        //to do: refactor
        fetch('/room_join', {
          method: 'POST',
          headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({
            chatroom: roomName,
            uname: store.getState().auth.uname
          })
        });
      }
    });   
  }

  return (
    <div className='item create-chat'>      
      <span>create chatroom</span>

      <input
        id='create_chatroom'        
        className='find-chatroom' 
        type='text'
        placeholder='chatroom'
      />

      <button
        onClick={createChatRoom}
        className='find-chatroom-btn'>
          ok
      </button>

    </div>
  );
}