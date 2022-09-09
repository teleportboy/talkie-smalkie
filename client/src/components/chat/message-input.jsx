import React from "react";
import { useSelector } from "react-redux";
import { store } from "../../store/store";

export function MessageInput() {
  const chatid = useSelector(state => state.currentChat.id);
  const uname = useSelector(state => state.auth.uname);
  
  const sendMessageHandler = () => {
    const message = {
      message: document.getElementById('message').value,
      chatroom: chatid,
      sender: uname
    };
    
    fetch('/message', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(message)
    })
      .then((response) => {
        return response.json();
      })
      .then(data => {
      });
  }
  
  return (
    <div className="item chat-input input-container">
      <textarea className="input-message" name="message" id="message" defaultValue={""} />
      <button onClick={sendMessageHandler} className="send-message">send</button>
    </div>
  );
}