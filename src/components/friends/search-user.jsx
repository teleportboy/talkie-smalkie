import React from "react";

export function SearchUser() {
  return (
    <div className="item profile-menu">
      <span>search</span>
      <input className="find-user" type="text" placeholder="find user" />
      <button className="find-user-btn">ok</button>
    </div>
  );
}