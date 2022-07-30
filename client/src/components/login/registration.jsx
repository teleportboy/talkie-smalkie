import React from "react";

export function Registration() {
  return (
    <form className="login">
      <div className="username-input item">
        <label htmlFor="uname"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="uname" required />
      </div>
      <div className="password-input item">
        <label htmlFor="psw"><b>Password</b></label>
        <input type="password" placeholder="Enter Password" name="psw" required />
      </div>
      <div className="login-btn item">
        <button type="submit"><b>Create</b></button>
      </div>
    </form>
  );
}