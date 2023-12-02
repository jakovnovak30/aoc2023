module Day1 where

import Data.Char (isNumber)
import Data.List (isPrefixOf)

getAsNumber :: String -> Int
getAsNumber = read . (\xs -> head xs : [last xs]) . keepNumbers

solution :: [String] -> Int
solution = sum . map getAsNumber

keepNumbers :: String -> String
keepNumbers = go "" "" . reverse
  where
    go result buffer [] = if not (null buffer) && isNumber (head $ convertNumber buffer) then head (convertNumber buffer) : result else result
    go result [] (curr : rest)
      | isNumber curr = go (curr : result) [] rest
      | otherwise = go result [curr] rest
    go result buffer (curr : rest)
      | isNumber $ head $ convertNumber buffer = go (convertNumber buffer ++ result) (curr : buffer) rest
      | isNumber curr = go (curr : result) buffer rest
      | otherwise = go result (curr : buffer) rest

convertNumber :: String -> String
convertNumber str
    | "one" `isPrefixOf` str = "1"
    | "two" `isPrefixOf` str = "2"
    | "three" `isPrefixOf` str = "3"
    | "four" `isPrefixOf` str = "4"
    | "five" `isPrefixOf` str = "5"
    | "six" `isPrefixOf` str = "6"
    | "seven" `isPrefixOf` str = "7"
    | "eight" `isPrefixOf` str = "8"
    | "nine" `isPrefixOf` str = "9"
    | otherwise = str
