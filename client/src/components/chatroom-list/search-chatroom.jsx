import React from "react";
import { useDispatch } from "react-redux";
import { updateSearchedChatRooms } from "../../store/search-chatrooms-slice.js"

export function SearchChatRoom() {
  const dispatch = useDispatch();

  const getChatRooms = () => {
    const searchValue = document.getElementById('search_chatroom').value;

    const url = new URL('http://127.0.0.1:8888/search_chatrooms');
    const params = { match: searchValue };
    url.search = new URLSearchParams(params).toString();

    fetch(url)
      .then(response => response.json())
      .then(data => {
        dispatch(updateSearchedChatRooms(data.chatrooms));
      });
  }

  return (
    <div className="item search-chatroom">
      <span>search chatroom</span>
      <input id="search_chatroom" className="find-chatroom" type="text" placeholder="search chatroom" />
      <button onClick={getChatRooms} className="find-chatroom-btn">ok</button>
    </div>
  );
}