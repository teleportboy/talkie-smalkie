import React from "react";

export function MessageInput() {
  const sendMessageHandler = () => {
    const message = {
      message: document.getElementById('message').value,
      receiver: 'teleportboy',
      sender: 'loading'
    };
    console.log(message);
    
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
        console.log(data);
      });
  }

  const get_message_body = {
    chat_uuid: 'test1-chq'
  };
  const getMessageHandler = () => {
    fetch('/message')    
      .then(response => {
        return response.json();
      })
      .then(data => {
        console.log(data);
      });
  }
  
  return (
    <div className="item chat-input input-container">
      <textarea className="input-message" name="message" id="message" defaultValue={""} />
      <button onClick={sendMessageHandler} className="send-message">send</button>
      <button onClick={getMessageHandler} className="audio-message">upd</button>
    </div>
  );
}