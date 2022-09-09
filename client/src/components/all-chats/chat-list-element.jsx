import React from "react";
import { useDispatch } from "react-redux";
import { reloadChat } from "../../store/current-chat-slice.js";
import { store } from "../../store/store.js";

export function ChatListItem({ chat_id }) {
  const dispatch = useDispatch();
  const changeChat = () => {
    const chat = store.getState().chatAreas.chats.find(
      (chat) => {
        return chat.id === chat_id;
      }
    );

    dispatch(reloadChat({
      id: chat_id,
      messages: chat.messages
    }));
  }

  const setHighlight = event => {
    event.target.classList.remove('item');
    event.target.classList.add('highlight-item');
  }
  const removeHighlight = event => {
    event.target.classList.remove('highlight-item');
    event.target.classList.add('item');
  }  

  return (
    <li
      className="item"
      onMouseOver={setHighlight}
      onMouseOut={removeHighlight}
      onClick={changeChat}
    >
      {chat_id}
    </li>
  );
}