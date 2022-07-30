import React from "react";
import { Friend } from "./friend.jsx";

export function FriendList() {
  return (
    <div className="item friends-menu">friends
      <ul className="friends-list">
        <Friend></Friend>
        <Friend></Friend>
        <Friend></Friend>
        <Friend></Friend>
      </ul>
    </div>
  )
}