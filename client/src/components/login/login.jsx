import React from "react";
import { useDispatch } from "react-redux";
import { useNavigate } from "react-router-dom";
import { store } from "../../store/store";
import { login } from "../../store/auth-slice";
import { addChatRoom } from "../../store/chatrooms-list-slice";
import { createChat, setupAllChats } from "../../store/chat-area-slice";
import { subscribeToChatRoom } from "../../sse-connection/conection";

export function Login() {
  const dispatch = useDispatch();
  const navigate = useNavigate();

  const onSubmit = (event) => {
    event.preventDefault();

    const formData = new FormData(event.target.parentNode);
    const formProps = Object.fromEntries(formData);
    console.log(formProps);

    fetch('/login', {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(formProps)
    }).then(response => {
      return response.json();
    }).then(data => {
      if (data.isOk === 'true') {
        dispatch(login(formProps.uname));
        navigate('/');

        let url = new URL('http://192.168.218.135:8888/all_chats');
        const params = { uname:  store.getState().auth.uname };
        url.search = new URLSearchParams(params).toString();
            
        fetch(url)
          .then(response => response.json())
          .then(data => {
            data.allChats.forEach(chat => {
              dispatch(addChatRoom(chat.chatroom));
              subscribeToChatRoom(chat.chatroom, dispatch);
            });
            
            dispatch(setupAllChats({
              allChats: data.allChats,
              uname: formProps.uname
            }));
          });
      }
    });
  }

  return (
    <form className="login" id="login_form">
      <div className="username-input item">
        <label htmlFor="uname"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="uname" required />
      </div>
      <div className="password-input item">
        <label htmlFor="psw"><b>Password</b></label>
        <input type="text" placeholder="Enter Password" name="psw" required />
      </div>
      
      <button onClick={onSubmit} type="submit">Loooogin</button>
      
      <div className="new-acc-btn item">
        <button type="button">Create new account</button>
      </div>
    </form>
  );
}